sudo singularity shell --writable -B /home/admin/.Xauthority:/root/.Xauthority -B /run/user/1000:/run/user/1000/ -B .:/home SIFs/DAQ_v3
