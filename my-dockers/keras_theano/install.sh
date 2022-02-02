#!/bin/bash -x

docker stop ml_keras_tf_theano_0 && docker rm ml_keras_tf_theano_0
docker run --runtime=nvidia \
	-v /mnt:/mnt \
	-p 8888:8888 -p 6006:6006 \
	--name ml_keras_tf_theano_0 --tmpfs /tmp ml_keras_tf_theano:latest 
	