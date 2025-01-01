# C++ code to interface with arduino

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
serial_monitor
http_getter
influxwrite

to run in background use 
nohup ./influxwrite & 


