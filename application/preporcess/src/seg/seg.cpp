#include "seg.hpp"
#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/features/normal_3d.h>
#include <pcl/features/normal_3d_omp.h>//使用OMP需要添加的头文件
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/filters/passthrough.h>  //直通滤波器头文件
#include <pcl/common/transforms.h>
#include <pcl/segmentation/extract_clusters.h>
#include <pcl/segmentation/region_growing.h>

#include <opencv2/opencv.hpp>
#include <Eigen/Core>
#include <Eigen/Geometry>
// extern loglevel_e loglevel;


// #include "../base/common.hpp"
namespace PointCloud_process{

    Segment::Segment(){
        // cloud_all=new pcl::PointCloud<PointRGB>;
        // pcl::PointCloud<PointRGB>::Ptr tmp(new pcl::PointCloud<PointRGB>);
        // cloud_all=tmp;
        cloud_all = pcl::PointCloud<PointRGB>::Ptr(new pcl::PointCloud<PointRGB>);
        viewer=boost::shared_ptr<pcl::visualization::PCLVisualizer>(new pcl::visualization::PCLVisualizer("planar segment"));

    }


cv::Mat Segment::projection(POINTCLOUD::Ptr cloud){

    //pcl_viewer '/home/hanglok/pc_ws/src/application/preporcess/dataset/1.pcd'  -ax 5

    cv::Mat depthMap = cv::Mat::zeros(Lidar_FOV_Height, Lidar_FOV_Width, CV_32FC1);
        // 遍历点云，更新深度图

#define Lidar_Axis_Trans 0
#if Lidar_Axis_Trans
    Eigen::Matrix4f transform = Eigen::Matrix4f::Identity();
    transform(0,0)=0;transform(0,1)=-1;transform(0,2)=0;transform(0,3)=0;
    transform(1,0)=1;transform(1,1)=0;transform(1,2)=0;transform(1,3)=0;
    transform(2,0)=0;transform(2,1)=0;transform(2,2)=1;transform(2,3)=0;
    transform(3,0)=0;transform(3,1)=0;transform(3,2)=0;transform(3,3)=1;
    // 应用变换
    POINTCLOUD::Ptr transformed_cloud(new POINTCLOUD());
    pcl::transformPointCloud(*cloud, *transformed_cloud, transform);

    for (const auto& point : transformed_cloud->points) {
#else
    for (const auto& point : cloud->points) {
#endif

#if Lidar_Axis_Trans
        // 假设点云已经在相机坐标系中
        int u = static_cast<int>(point.x * Lidar_Focal_Length / point.z + Lidar_FOV_Width / 2);
        int v = static_cast<int>(point.y * Lidar_Focal_Length / point.z + Lidar_FOV_Height / 2);
#else
        // 机器人坐标系
        int u = static_cast<int>(-point.y * Lidar_Focal_Length / point.x + Lidar_FOV_Width / 2);
        int v = static_cast<int>(-point.z * Lidar_Focal_Length / point.x + Lidar_FOV_Height / 2);
#endif
        
        if (u >= 0 && u < Lidar_FOV_Width && v >= 0 && v < Lidar_FOV_Height) {
            depthMap.at<float>(v, u) = point.x;
            std::cout << u << ", "<< v << std::endl;
        }
    }
    // 转换深度图为可视化格式（可选）
    cv::Mat displayMap;
    cv::normalize(depthMap, displayMap, 0, 255, cv::NORM_MINMAX, CV_8UC1);
    cv::imwrite("/home/uestc/pc_ws/src/1.png",displayMap);
    cv::imshow("Depth Map", displayMap);
    cv::waitKey(0);
    return depthMap;

}


void Segment::backprojection(POINTCLOUD::Ptr cloud){

}

//直通滤波器对点云进行处理
void Segment::cloudPassThrough(POINTCLOUD::Ptr cloud,const char *axis,int min,int max)
{
     pcl::PassThrough<POINTTYPE> passthrough;
     passthrough.setInputCloud(cloud);//输入点云
     passthrough.setFilterFieldName(axis);//对z轴进行操作
     passthrough.setFilterLimits(min,max);//设置直通滤波器操作范围
     passthrough.filter(*cloud);//);//执行滤波

}

void Segment::normal_viz(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_input)
{
    // pointcloud_size<POINTTYPE>(cloud);

    // 创建法向量估计对象
    // pcl::NormalEstimation<POINTTYPE, pcl::Normal> ne;
    pcl::NormalEstimationOMP<POINTTYPE, pcl::Normal> ne;//OMP加速
    ne.setInputCloud(cloud_input);
    ne.setNumberOfThreads(10);//设置openMP的线程数


    // 估计法向量
    pcl::search::KdTree<POINTTYPE>::Ptr tree(new pcl::search::KdTree<POINTTYPE>);
    ne.setSearchMethod(tree);
    pcl::PointCloud<pcl::Normal>::Ptr cloud_normals(new pcl::PointCloud<pcl::Normal>);
    ne.setRadiusSearch(0.3);  // 设置法向量估计半径
    // ne.setKSearch(10);//点云法向计算时，需要所搜的近邻点大小
    ne.compute(*cloud_normals);
    viewer->addPointCloud<pcl::PointXYZ>(cloud_input, "cloud");


    // 添加法线到可视化
    viewer->addPointCloudNormals<pcl::PointXYZ, pcl::Normal>(cloud_input, cloud_normals, 10, 0.05, "normals");


}

void Segment::Plane_fitting_cluster_growth(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_input)
{

    pcl::PointCloud<pcl::Normal>::Ptr cloud_normals(new pcl::PointCloud<pcl::Normal>);
    pcl::search::KdTree<POINTTYPE>::Ptr tree(new pcl::search::KdTree<POINTTYPE>);
    {
        pcl::NormalEstimationOMP<POINTTYPE, pcl::Normal> ne;//OMP加速
        ne.setInputCloud(cloud_input);
        ne.setNumberOfThreads(10);//设置openMP的线程数
        // 估计法向量
        
        ne.setSearchMethod(tree);
        
        ne.setRadiusSearch(0.3);  // 设置法向量估计半径
        // ne.setKSearch(10);//点云法向计算时，需要所搜的近邻点大小
        ne.compute(*cloud_normals);
    }    

    //区域增长聚类分割对象	< 点 ， 法 线 > 
    pcl::RegionGrowing<pcl::PointXYZ, pcl::Normal> reg; 
    reg.setMinClusterSize (50);	//最小的聚类的点数
    reg.setMaxClusterSize (1000000);//最大的聚类的点数
    reg.setSearchMethod (tree);	//搜索方式
    reg.setNumberOfNeighbours (30); //设置搜索的邻域点的个数
    reg.setInputCloud (cloud_input);	//输入点
    //reg.setIndices (indices); 
    reg.setInputNormals (cloud_normals);	//输入的法线
    reg.setSmoothnessThreshold (3.0 / 180.0 * M_PI);//设置平滑度 法线差值阈值
    reg.setCurvatureThreshold (1.0);	//设置曲率的阀值

        // 获取聚类结果
    std::vector<pcl::PointIndices> clusters;
    reg.extract(clusters);

    // 遍历和处理每个聚类
    int j = 0;    
    int m = 0;
    for (const auto& cluster : clusters) {
        pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_cluster(new pcl::PointCloud<pcl::PointXYZ>);
        for (const auto& idx : cluster.indices)
            cloud_cluster->points.push_back(cloud_input->points[idx]); // 添加点到当前簇
        cloud_cluster->width = cloud_cluster->points.size();
        cloud_cluster->height = 1;
        cloud_cluster->is_dense = true;

        if (cloud_cluster->size()>40)
        {
            m++;
            // 可视化相关的代码
            R = rand() % (256) + 0;
            G = rand() % (256) + 0;
            B = rand() % (256) + 0;
            cout<<"cloud_cluster->size()="<<cloud_cluster->size()<<endl;
            pcl::PointCloud<PointRGB>::Ptr cloud_cluster_vis(new pcl::PointCloud<PointRGB>);
            for(std::size_t k=0; k<cloud_cluster->size(); k++ )
            {
                PointRGB thispoint;
                thispoint.x=cloud_cluster->points[k].x;
                thispoint.y=cloud_cluster->points[k].y;
                thispoint.z=cloud_cluster->points[k].z;
                thispoint.r=R;
                thispoint.g=G;
                thispoint.b=B;
                cloud_cluster_vis->push_back(thispoint);
                cloud_all->push_back(thispoint);
            }
            output_plane(cloud_cluster_vis,m);
        }
    }     
}

void Segment::Plane_fitting_cluster_eu(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_input)
{

    // 创建KdTree对象
    pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>);
    tree->setInputCloud(cloud_input);

    // 创建一个用于提取聚类的EuclideanClusterExtraction对象
    std::vector<pcl::PointIndices> cluster_indices;
    pcl::EuclideanClusterExtraction<pcl::PointXYZ> ec;
    ec.setClusterTolerance(0.8); // 2cm
    ec.setMinClusterSize(100);
    ec.setMaxClusterSize(50000);
    ec.setSearchMethod(tree);
    ec.setInputCloud(cloud_input);
    ec.extract(cluster_indices);

    int m = 0;
    // 遍历每个簇
    int j = 0;
    for (std::vector<pcl::PointIndices>::const_iterator it = cluster_indices.begin(); it != cluster_indices.end(); ++it) {
        pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_cluster(new pcl::PointCloud<pcl::PointXYZ>);
        for (const auto& idx : it->indices)
            cloud_cluster->points.push_back(cloud_input->points[idx]); // 添加点到当前簇
        cloud_cluster->width = cloud_cluster->points.size();
        cloud_cluster->height = 1;
        cloud_cluster->is_dense = true;

        // std::cout << "PointCloud representing the Cluster: " << cloud_cluster->points.size() << " data points." << std::endl;
        // std::stringstream ss;
        // ss << "cloud_cluster_" << j << ".pcd";
        // pcl::io::savePCDFileASCII(ss.str(), *cloud_cluster); // 保存簇

        // if(cloud_cluster->size()<100){
        //     std::cout << "plane less than 100, exit."  << std::endl;
        //     break;
        // }
        if (cloud_cluster->size()>40)
        {
            m++;
            // 可视化相关的代码
            R = rand() % (256) + 0;
            G = rand() % (256) + 0;
            B = rand() % (256) + 0;
            cout<<"cloud_cluster->size()="<<cloud_cluster->size()<<endl;
            pcl::PointCloud<PointRGB>::Ptr cloud_cluster_vis(new pcl::PointCloud<PointRGB>);
            for(std::size_t k=0; k<cloud_cluster->size(); k++ )
            {
                PointRGB thispoint;
                thispoint.x=cloud_cluster->points[k].x;
                thispoint.y=cloud_cluster->points[k].y;
                thispoint.z=cloud_cluster->points[k].z;
                thispoint.r=R;
                thispoint.g=G;
                thispoint.b=B;
                cloud_cluster_vis->push_back(thispoint);
                cloud_all->push_back(thispoint);
            }
            output_plane(cloud_cluster_vis,m);
        }
        j++;
    }


}

void Segment::Plane_fitting_normal(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_input)
{
    // pointcloud_size<POINTTYPE>(cloud);

    // 创建法向量估计对象
    pcl::NormalEstimationOMP<POINTTYPE, pcl::Normal> ne;//OMP加速
    ne.setInputCloud(cloud_input);
    ne.setNumberOfThreads(10);//设置openMP的线程数


    // 估计法向量
    pcl::search::KdTree<POINTTYPE>::Ptr tree(new pcl::search::KdTree<POINTTYPE>);
    ne.setSearchMethod(tree);
    pcl::PointCloud<pcl::Normal>::Ptr cloud_normals(new pcl::PointCloud<pcl::Normal>);
    ne.setRadiusSearch(0.3);  // 设置法向量估计半径
    // ne.setKSearch(10);//点云法向计算时，需要所搜的近邻点大小
    ne.compute(*cloud_normals);

    // logit(logDEBUG3) << cloud_normals->width <<","<< cloud_normals->height <<","<< sizeof(cloud_normals->points[0]);
    pointcloud_size<pcl::Normal>(cloud_normals);
    // pointcloud_size(cloud_normals);

    // 创建分割对象
    pcl::SACSegmentationFromNormals<POINTTYPE, pcl::Normal> seg;
    seg.setOptimizeCoefficients(true);
    seg.setModelType(pcl::SACMODEL_NORMAL_PLANE);
    seg.setMethodType(pcl::SAC_RANSAC);
    seg.setMaxIterations(100);
    seg.setDistanceThreshold(DisThre);  // 设置距离阈值

    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_plane(new pcl::PointCloud<pcl::PointXYZ>());


    // 设置输入点云和法向量
    seg.setInputCloud(cloud_input);
    seg.setInputNormals(cloud_normals);

    // 创建一个对象来存储分割结果
    pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients);
    pcl::PointIndices::Ptr inliers(new pcl::PointIndices);


    // std::cout << "分割结果: " << inliers->indices.size() << " 个点被识别为平面" << std::endl;
	// //判断是否分割成功
	// if (inliers->indices.size() == 0)
	// {
	// 	PCL_ERROR("Could not estimate a planar model for the given dataset.");
	// 	return (-1);
	// }
	// std::cerr << std::endl << "Model coefficients: " << coefficients->values[0] << " "
	// 	<< coefficients->values[1] << " "
	// 	<< coefficients->values[2] << " "
	// 	<< coefficients->values[3] << std::endl << std::endl;


    int m=0;
    while (cloud_input->size() > 100)
    {
        pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_p(new pcl::PointCloud<pcl::PointXYZ>);
        pcl::PointCloud<pcl::Normal>::Ptr cloud_norm_p(new pcl::PointCloud<pcl::Normal>);
        // 设置输入点云和法向量
        seg.setInputCloud(cloud_input);
        seg.setInputNormals(cloud_normals);

        // 分割平面
        seg.segment(*inliers, *coefficients);


        if (inliers->indices.size() == 0)
        {
            PCL_ERROR("Could not estimate a planar model for the given dataset.");
            break;
        }
        std::cout << "remaining points: " << cloud_input->size() << std::endl;
        pcl::ExtractIndices<pcl::PointXYZ> extract;
        extract.setInputCloud(cloud_input);
        extract.setIndices(inliers);
        extract.filter(*cloud_plane);//输出平面

        pcl::ExtractIndices<pcl::Normal> extract_normal;
        extract_normal.setInputCloud(cloud_normals);
        extract_normal.setIndices(inliers);
        // extract_normal.filter(*cloud_plane_normals);//输出平面
        if(cloud_plane->size()<100){
            std::cout << "plane less than 100, exit."  << std::endl;
            break;
        }
        if (cloud_plane->size()>40)
        {
            m++;
            // 可视化相关的代码
            R = rand() % (256) + 0;
            G = rand() % (256) + 0;
            B = rand() % (256) + 0;
            cout<<"cloud_cluster->size()="<<cloud_plane->size()<<endl;
            pcl::PointCloud<PointRGB>::Ptr cloud_cluster(new pcl::PointCloud<PointRGB>);
            for(std::size_t k=0; k<cloud_plane->size(); k++ )
            {
                PointRGB thispoint;
                thispoint.x=cloud_plane->points[k].x;
                thispoint.y=cloud_plane->points[k].y;
                thispoint.z=cloud_plane->points[k].z;
                thispoint.r=R;
                thispoint.g=G;
                thispoint.b=B;
                cloud_cluster->push_back(thispoint);
                cloud_all->push_back(thispoint);
            }
            output_plane(cloud_cluster,m);
        }

        // 移除plane
        extract.setNegative(true);
        extract.filter(*cloud_p);
        *cloud_input = *cloud_p;
        
        extract_normal.setNegative(true);
        extract_normal.filter(*cloud_norm_p);
        *cloud_normals = *cloud_norm_p;

    }

}

void Segment::Plane_fitting(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_input)
{

    pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients);
    pcl::PointIndices::Ptr inliers(new pcl::PointIndices);
    pcl::SACSegmentation<pcl::PointXYZ> seg;
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_plane(new pcl::PointCloud<pcl::PointXYZ>());

    seg.setOptimizeCoefficients(true);
    seg.setModelType(pcl::SACMODEL_PLANE);
    seg.setMethodType(pcl::SAC_RANSAC);
    seg.setMaxIterations(300);
    seg.setDistanceThreshold(DisThre);

    int m=0;
    while (cloud_input->size() > 100)
    {
        pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_p(new pcl::PointCloud<pcl::PointXYZ>);
        seg.setInputCloud(cloud_input);
        seg.segment(*inliers, *coefficients);
        if (inliers->indices.size() == 0)
        {
            break;
        }
        pcl::ExtractIndices<pcl::PointXYZ> extract;
        extract.setInputCloud(cloud_input);
        extract.setIndices(inliers);
        extract.filter(*cloud_plane);//输出平面

        if (cloud_plane->size()>40)
        {
            m++;
            // 可视化相关的代码
            R = rand() % (256) + 0;
            G = rand() % (256) + 0;
            B = rand() % (256) + 0;
            cout<<"cloud_cluster->size()="<<cloud_plane->size()<<endl;
            pcl::PointCloud<PointRGB>::Ptr cloud_cluster(new pcl::PointCloud<PointRGB>);
            for(std::size_t k=0; k<cloud_plane->size(); k++ )
            {
                PointRGB thispoint;
                thispoint.x=cloud_plane->points[k].x;
                thispoint.y=cloud_plane->points[k].y;
                thispoint.z=cloud_plane->points[k].z;
                thispoint.r=R;
                thispoint.g=G;
                thispoint.b=B;
                cloud_cluster->push_back(thispoint);
                cloud_all->push_back(thispoint);
            }
            output_plane(cloud_cluster,m);
        }
        // 移除plane
        extract.setNegative(true);
        extract.filter(*cloud_p);
        *cloud_input = *cloud_p;
    }

}

void Segment::output_plane(pcl::PointCloud<PointRGB>::Ptr cloud_plane,int begin)
{
    std::stringstream ss;
    ss << "plane_" << begin<< ".pcd";
    pcl::io::savePCDFileBinary(ss.str(), *cloud_plane);
    cout << ss.str() << "保存完毕！！！" << endl;

    std::string str;
    ss >> str;
    pcl::visualization::PointCloudColorHandlerCustom<PointRGB> color(cloud_plane, R, G, B);
    viewer->addPointCloud<PointRGB>(cloud_plane, color, str);
    viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, str);
}

};
