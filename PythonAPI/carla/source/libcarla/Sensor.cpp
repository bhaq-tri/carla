// Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#include <carla/PythonUtil.h>
#include <carla/client/ClientSideSensor.h>
#include <carla/client/LaneInvasionSensor.h>
#include <carla/client/DataLineSensor.h>
#include <carla/client/RaceLineSensor.h>
#include <carla/client/Sensor.h>
#include <carla/client/ServerSideSensor.h>
#include <carla/Logging.h>

static void SubscribeToStream(carla::client::Sensor &self, boost::python::object callback) {
  self.Listen(MakeCallback(std::move(callback)));
}

static void PythonUpdateSpline(carla::client::RaceLineSensor& self, boost::python::list input) {
  carla::log_info("PythonUpdateSpline called");
  self.UpdateSpline(PythonLitstToVector<carla::geom::Vector3D>(input));
}

static void UpdateMyDataPython(carla::client::DataLineSensor& self, boost::python::list input) {
  carla::log_error("UpdateMyDataPython called");
  self.UpdateSplineData(PythonLitstToVector<int>(input));
}
static void UpdateRawDataPython(carla::client::DataLineSensor& self, int input) {
  carla::log_error("UpdatRawDataPython called failed here?");
  self.UpdateRawData(input);
}

void export_sensor() {
  using namespace boost::python;
  namespace cc = carla::client;

  class_<cc::Sensor, bases<cc::Actor>, boost::noncopyable, boost::shared_ptr<cc::Sensor>>("Sensor", no_init)
    .add_property("is_listening", &cc::Sensor::IsListening)
    .def("listen", &SubscribeToStream, (arg("callback")))
    .def("stop", &cc::Sensor::Stop)
    .def(self_ns::str(self_ns::self))
  ;

  class_<cc::ServerSideSensor, bases<cc::Sensor>, boost::noncopyable, boost::shared_ptr<cc::ServerSideSensor>>
      ("ServerSideSensor", no_init)
    .def(self_ns::str(self_ns::self))
  ;

  class_<cc::ClientSideSensor, bases<cc::Sensor>, boost::noncopyable, boost::shared_ptr<cc::ClientSideSensor>>
      ("ClientSideSensor", no_init)
    .def(self_ns::str(self_ns::self))
  ;

  class_<cc::LaneInvasionSensor, bases<cc::ClientSideSensor>, boost::noncopyable, boost::shared_ptr<cc::LaneInvasionSensor>>
      ("LaneInvasionSensor", no_init)
    .def(self_ns::str(self_ns::self))
  ;

  class_<cc::DataLineSensor, bases<cc::ClientSideSensor>, boost::noncopyable, boost::shared_ptr<cc::DataLineSensor>>
      ("DataLineSensor", no_init)
    .def(self_ns::str(self_ns::self))
    .def("update_my_data", &UpdateMyDataPython, (arg("data")))
    .def("update_raw_data", &UpdateRawDataPython, (arg("data")))
  ;

  class_<cc::RaceLineSensor, bases<cc::Sensor>, boost::noncopyable, boost::shared_ptr<cc::RaceLineSensor>>
      ("RaceLineSensor", no_init)
    .def(self_ns::str(self_ns::self))
    .def("update_spline", &PythonUpdateSpline, (arg("data")))
  ;
}
