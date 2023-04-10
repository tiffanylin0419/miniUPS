#make sure host name at db_param.h file is mydb instead of localhost
FROM yunjingliu/world_sim:base
ADD . /worldsim
WORKDIR /worldsim
RUN chmod u+r+x wait-for-it.sh
RUN chmod u+r+x ./server