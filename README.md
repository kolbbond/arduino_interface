# C++ code to interface with arduino
Goal to read data from arduino and write to influx database
and remotely track using Grafana

## Dependencies
This requires 
openssl
curl
boost (make sure you get the latest version check 'sudo apt-cache search libboost')

ADD TCLAP

Since we are running these primarily on a raspberry pi

sudo apt install libssl-dev libcurl4-openssl-dev libboost#.##-all-dev

Then run './scripts/install_deps.sh'
to install the other dependencies

build like cmake

## programs in examples
serial_monitor -> read from serial port (/dev/ttyUSB0 etc.)
http_getter -> read over http 
influxwrite -> read over http and write to influx

to run in background use 
nohup ./influxwrite & 

You can set up grafana to access the database and plot remotely.

