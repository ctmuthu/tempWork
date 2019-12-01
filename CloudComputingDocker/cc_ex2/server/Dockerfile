FROM node:boron
RUN mkdir -p /usr/src/cloudcomputinggroup45
WORKDIR /usr/src/cloudcomputinggroup45
COPY package.json /usr/src/cloudcomputinggroup45
RUN npm install
COPY . /usr/src/cloudcomputinggroup45
EXPOSE 3008
CMD ["node","server.js"]
