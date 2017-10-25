https://blog.alexellis.io/getting-started-with-docker-on-raspberry-pi/
https://www.vultr.com/docs/how-to-use-docker-creating-your-first-docker-container

Installing

curl -sSL https://get.docker.com | sh


Set Docker to auto-start
sudo systemctl enable docker


You can now reboot the Pi, or start the Docker daemon with:
sudo systemctl start docker


The Docker client can only be used by root or members of the docker group. Add pi or your equivalent user to the docker group:
sudo usermod -aG docker pi

After making this change, log out and reconnect with ssh.



Support for ARM and the Raspberry Pi is a work-in-progress item which means there are a few things you should know.

Pulling images from the Hub
If you pull down the busybox image from the Docker hub it will not function correctly. This is because the contents are designed for a regular PC or the x86_64 architecture. There are on-going efforts to fix this in future versions of Docker.






## I2c in Docker

As a security precaution, system devices are not exposed by default inside Docker containers. You can expose specific devices to your container using the --device option to docker run, as in:

docker run --device /dev/i2c-0 --device /dev/i2c-1 myimage
You can remove all restrictions with the --privileged flag:

docker run --privileged myimage
This will expose all of /dev to your container, and remove other restrictions as well (e.g., you will be able to change the network configuration in the container and mount new filesystems).


## build

docker build -t home_automator .

## running

docker run -d --privileged home_automator
