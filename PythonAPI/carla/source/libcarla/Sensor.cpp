// Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#include <carla/PythonUtil.h>
#include <carla/client/ClientSideSensor.h>
#include <carla/client/LaneInvasionSensor.h>
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

static void PythonUpdateColor(carla::client::RaceLineSensor& self, boost::python::list input) {
  carla::log_info("PythonUpdateColor called");
  self.SetColor(PythonLitstToVector<float>(input));
}

static void PythonSetArrowHorizontalOffset(carla::client::RaceLineSensor& self, float input) {
  carla::log_info("PythonSetArrowHorizontalOffset called");
  self.SetArrowHorizontalOffset(input);
}

static void PythonSetArrowVerticalOffset(carla::client::RaceLineSensor& self, float input) {
  carla::log_info("PythonSetArrowVerticalOffset called");
  self.SetArrowVerticalOffset(input);
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

  class_<cc::RaceLineSensor, bases<cc::Sensor>, boost::noncopyable, boost::shared_ptr<cc::RaceLineSensor>>
      ("RaceLineSensor", no_init)
    .def(self_ns::str(self_ns::self))
    .def("update_spline", &PythonUpdateSpline, (arg("data")))
    .def("set_spline", &PythonUpdateSpline, (arg("data")))
    .def("set_color_array", &PythonUpdateColor, (arg("data")))
    .def("set_arrow_horizontal_offset", &PythonSetArrowHorizontalOffset, (arg("data")))
    .def("set_arrow_vertical_offset", &PythonSetArrowVerticalOffset, (arg("data")))
  ;
}
