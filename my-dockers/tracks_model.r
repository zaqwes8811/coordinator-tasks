# RUN:
# export RSTUDIO_WHICH_R=/mnt/ssd0_sys/R-patched/bin/R rstudio &
 
# to x = f(t)
# https://www.datanovia.com/en/lessons/select-data-frame-columns-in-r/
# BUG: https://github.com/tidyverse/readr/issues/919
# 
# R shared library (/usr/local/lib/R/lib/libR.so) not found. If this is a 
# custom build of R, was it built with the --enable-R-shlib option?

# ./configure --enable-R-shlib
#
# wget https://download1.rstudio.org/rstudio-xenial-1.1.463-amd64.deb
# wget wget http://download1.rstudio.org/rstudio-1.0.153-amd64.deb
# R version 3.2.3 (2015-12-10) -- "Wooden Christmas-Tree"
# sudo apt-get install xorg-dev
# wget https://cran.r-project.org/src/base-prerelease/R-patched_2018-12-04_r75765.tar.gz
# rstudio: /usr/lib/x86_64-linux-gnu/libstdc++.so.6: version `GLIBCXX_3.4.20' not found (required by rstudio)
#install.packages("readr")
#install.packages("haven")
#install.packages("tidyverse")  # looooong time

# our data
library(tidyverse)
library(forecast)

#install.packages("forecast")

fn <- '/tmp/ttttttt.csv'
df <- read.table(fn, header = TRUE, sep = ",")

#my_data <- as_tibble(df)

t_to_x_df <- df %>% select("T", "Y")

acf(t_to_x_df, lag.max=20)
pacf(t_to_x_df, lag.max=8)  # нужно знать длину

# https://stackoverflow.com/questions/43622486/time-series-forecasting-in-r-univariate-time-series
pricearima <- ts(t_to_x_df)#, frequency = 12)
#adenoTS = ts(adeno)
arima_fit = auto.arima(pricearima[,1])
#fitlnstock<-auto.arima(pricearima)

forecastedvalues_ln=forecast(arima_fit,h=26)
plot(forecastedvalues_ln)

air <- window(ts(t_to_x_df))#, start=1990)
fc <- holt(air[,1], h=5)
air <- air[,1]

fc <- holt(air, h=15)
fc2 <- holt(air, damped=TRUE, phi = 0.9, h=15)
autoplot(air) +
  autolayer(fc, series="Holt's method", PI=FALSE) +
  autolayer(fc2, series="Damped Holt's method", PI=FALSE) +
  ggtitle("Forecasts from Holt's method") + xlab("Year") +
  ylab("Air passengers in Australia (millions)") +
  guides(colour=guide_legend(title="Forecast"))

