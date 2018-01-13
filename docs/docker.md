# Installing Docker on the RPi

Sources:
* [https://blog.alexellis.io/getting-started-with-docker-on-raspberry-pi/](https://blog.alexellis.io/getting-started-with-docker-on-raspberry-pi/)
* [https://www.vultr.com/docs/how-to-use-docker-creating-your-first-docker-container](https://www.vultr.com/docs/how-to-use-docker-creating-your-first-docker-container)

## Setup

Installing docker
```shell
curl -sSL https://get.docker.com | sh
```

Set Docker to auto-start
```shell
sudo systemctl enable docker
```

You can now reboot the Pi, or start the Docker daemon with:
```shell
sudo systemctl start docker
```

The Docker client can only be used by root or members of the docker group. Add pi or your equivalent user to the docker group:
```shell
sudo usermod -aG docker pi
```

After making this change, log out and reconnect with ssh.

## I2c in Docker

As a security precaution, system devices are not exposed by default inside Docker containers. You can expose specific devices to your container using the --device option to docker run, as in:
```shell
docker run --device /dev/i2c-0 --device /dev/i2c-1 myimage
```

You can remove all restrictions with the --privileged flag:
```shell
docker run --privileged myimage
```

This will expose all of /dev to your container, and remove other restrictions as well (e.g., you will be able to change the network configuration in the container and mount new filesystems).
