#include <map>
#include<iostream>
#include<map>

#include <rplidar.h>

using namespace rp::standalone::rplidar;

static RPlidarDriver* drv;
static u_result result;


void startLidar(bool& lidarstart) {

	const char* opt_com_path = "com6";

	// create the driver instance
	drv = RPlidarDriver::CreateDriver(DRIVER_TYPE_SERIALPORT);
	if (!drv) {
		exit(-2);
	}

	rplidar_response_device_info_t devinfo;

	if (!drv)
		drv = RPlidarDriver::CreateDriver(DRIVER_TYPE_SERIALPORT);
	if (IS_OK(drv->connect(opt_com_path, 115200)))
	{
		result = drv->getDeviceInfo(devinfo);

		if (IS_OK(result))
		{
			lidarstart = true;

			for (int pos = 0; pos < 16; ++pos) {
				printf("%02X", devinfo.serialnum[pos]);
			}
			printf("\n");

			printf("Firmware Ver: %d.%02d\n"
				"Hardware Rev: %d\n"
				, devinfo.firmware_version >> 8
				, devinfo.firmware_version & 0xFF
				, (int)devinfo.hardware_version);

		}
		else
		{
			std::cout << "Error Code: " << result << std::endl;
			delete drv;
			drv = nullptr;
		}
	}
	else {
		std::cout << "Lidar start failed" << std::endl;
		lidarstart = false;
		return;
	}


	std::vector<RplidarScanMode> outModes;
	drv->getAllSupportedScanModes(outModes);

	for (auto& i : outModes) {
		std::cout << (int)i.ans_type << std::endl;
		std::cout << (int)i.id << std::endl;
	}
	drv->startMotor();

	drv->startScanExpress(0, 2);

}


void getLidarData(int* data) {

	rplidar_response_measurement_node_hq_t nodes[8192];

	size_t nodeCount = sizeof(nodes) / sizeof(rplidar_response_measurement_node_hq_t);

	result = drv->grabScanDataHq(nodes, nodeCount);

	if (true) {
		drv->ascendScanData(nodes, nodeCount);
		std::cout << (int)nodeCount << std::endl;
		for (int i = 0; i < (int)nodeCount; i++)
		{

			float angle = (nodes[i].angle_z_q14 * 90.f / (1 << 14));
			if ((int)nodes[i].quality >= 60)
			{
				data[(int)angle] = nodes[i].dist_mm_q2 / (1 << 2);
			}
		}
	}
}