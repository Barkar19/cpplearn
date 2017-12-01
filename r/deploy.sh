docker build -t r-base-libs .
docker run -a stdin -a stdout -i -t -t r-base-libs R -f /home/cluster.r
