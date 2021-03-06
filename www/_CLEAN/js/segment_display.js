function pad (str, max) {
  str = str.toString();
  return str.length < max ? pad("0" + str, max) : str;
}
var display_odometry_left = new SegmentDisplay("display_odometry_left");
display_odometry_left.pattern         = "######";
display_odometry_left.displayAngle    = 7;
display_odometry_left.digitHeight     = 20;
display_odometry_left.digitWidth      = 14;
display_odometry_left.digitDistance   = 2.5;
display_odometry_left.segmentWidth    = 2;
display_odometry_left.segmentDistance = 0.3;
display_odometry_left.segmentCount    = 7;
display_odometry_left.cornerType      = 3;
display_odometry_left.colorOn         = "#090909";
display_odometry_left.colorOff        = "#C8C8C8";
display_odometry_left.draw();
display_odometry_left.setValue('--------');
var display_odometry_right = new SegmentDisplay("display_odometry_right");
display_odometry_right.pattern         = "######";
display_odometry_right.displayAngle    = 7;
display_odometry_right.digitHeight     = 20;
display_odometry_right.digitWidth      = 14;
display_odometry_right.digitDistance   = 2.5;
display_odometry_right.segmentWidth    = 2;
display_odometry_right.segmentDistance = 0.3;
display_odometry_right.segmentCount    = 7;
display_odometry_right.cornerType      = 3;
display_odometry_right.colorOn         = "#090909";
display_odometry_right.colorOff        = "#C8C8C8";
display_odometry_right.draw();
display_odometry_right.setValue('--------');
var display_intensite = new SegmentDisplay("display_intensite");
display_intensite.pattern         = "##.#";
display_intensite.displayAngle    = 7;
display_intensite.digitHeight     = 20;
display_intensite.digitWidth      = 14;
display_intensite.digitDistance   = 2.5;
display_intensite.segmentWidth    = 2;
display_intensite.segmentDistance = 0.3;
display_intensite.segmentCount    = 7;
display_intensite.cornerType      = 3;
display_intensite.colorOn         = "#FF4545";
display_intensite.colorOff        = "#C8C8C8";
display_intensite.draw();
display_intensite.setValue('----');