sudo docker stop $(sudo docker ps -a -q)
sudo docker rm $(sudo docker ps -a -q)
sudo docker rmi -f $(sudo docker images -a -q)
sudo docker build -t "microservice:server" ./server/
sudo docker build -t "microservice:hello" ./hello-world-service/
sudo docker build -t "microservice:productdescp" ./product-descp-service/
sudo docker build -t "microservice:productprice" ./product-price-service/
sudo docker tag $(sudo docker images -a -q microservice:server) aditideshpande/microservice:server
sudo docker tag $(sudo docker images -a -q microservice:hello) aditideshpande/microservice:hello
sudo docker tag $(sudo docker images -a -q microservice:productprice) aditideshpande/microservice:productprice
sudo docker tag $(sudo docker images -a -q microservice:productdescp) aditideshpande/microservice:productdescp
sudo docker login
sudo docker push aditideshpande/microservice
sudo docker rmi -f $(sudo docker images -a -q)
sudo docker-compose up
