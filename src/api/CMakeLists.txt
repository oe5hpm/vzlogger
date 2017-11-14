# -*- mode: cmake; -*-


set(api_srcs
  Volkszaehler.cpp
  MySmartGrid.cpp
  InfluxDB.cpp
  Null.cpp
  CurlIF.cpp
  CurlCallback.cpp
  CurlResponse.cpp
  Hpm-Test.cpp
)

add_library(vz-api ${api_srcs})
