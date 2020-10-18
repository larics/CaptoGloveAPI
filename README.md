# CaptoGloveAPI 


CaptoGloveAPI is cpp wrapper for methods for using Capto Gloves with the given SDK. 


# Dependencies 

Depends on `pthread` library which must be used and gcc 7.5.0

### How to install gcc 7.5.0?

```
sudo apt-get install -y software-properties-common
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt update
sudo apt install g++-7 -y
```

Add symbolic pointers to new version: 

```
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 60 \
                         --slave /usr/bin/g++ g++ /usr/bin/g++-7 
sudo update-alternatives --config gcc
gcc --version
g++ --version
```


## TODO: 
- [ ] Find account and explore more about BlueZ library 
- [ ] Find work-around for BLE implementation 
- [ ] Write CaptoGloveAPI 
- [ ] Write ros wrapper for CaptoGloveAPI 

