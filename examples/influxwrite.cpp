#include <InfluxDB/InfluxDB.h>
#include <InfluxDB/InfluxDBFactory.h>
#include <InfluxDB/Point.h>
#include <iostream>
#include <thread>
#include <chrono>

int main() {
	while(true) {

		// timer
		auto start = std::chrono::steady_clock::now();

		// attempt write to influx
		try {

			// Initialize InfluxDB connection
			// we do this each loop incase the connection drops?
			auto influxdb = influxdb::InfluxDBFactory::Get("http://10.0.0.33:8086?db=test");

			// Write a point to the database
			influxdb->write(influxdb::Point{"testname"}
					.addTag("location", "office")
					.addField("value", 22.5)
					.setTimestamp(std::chrono::system_clock::now()));

			std::cout << "Point written to InfluxDB successfully!" << std::endl;

			// Query data from the database
            // debug debug debug
            /*
			auto points = influxdb->query("SELECT * FROM testname WHERE location = 'office'");

			// Print the queried points
			for(const auto& point : points) {
				std::cout << point.getFields() << std::endl;
			}
            */


		} catch(const std::exception& e) {
			std::cerr << "Error: " << e.what() << std::endl;
		}

			// check clock
			auto end = std::chrono::steady_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

			// Sleep for the remainder of the second
			if(duration < std::chrono::milliseconds(1000)) {
				std::this_thread::sleep_for(std::chrono::milliseconds(1000) - duration);
			}
	}

	return 0;
}
