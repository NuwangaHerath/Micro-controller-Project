<!DOCTYPE html>
<html>
<head>
<title>Dashboard</title>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
body {
  margin: 0;
  background-color: #E1E2E8; 
}

/* Style the header */
.header {
  background-color: #0127E7;
  padding: 1px;
  text-align: center;
}
.column {
  float: left;
  width: 100%;
  padding: 15px;
  background-color: #CCCED8;
}
.row:after {
  content: "";
  display: table;
  clear: both;
}
@media screen and (max-width:600px) {
  .column {
    width: 100%;
  }
}
</style>
</head>

<body>
<div class="header">
<h1>Environment Monitor</h1>
</div>
<?php
//Creates new record as per request
    
    //Get current date and time
    date_default_timezone_set('Asia/colombo');
    $d = date("Y-m-d");
    $t = date("H:i:s");

    if(!empty($_POST['mtemp']) && !empty($_POST['mhum']) && !empty($_POST['mintensity']) && !empty($_POST['mpres']))
    {
    	$mtemp = $_POST['mtemp'];
      $mhum = $_POST['mhum'];
      $mintensity = $_POST['mintensity'];
      $mpres = $_POST['mpres'];
      $sdtemp = $_POST['sdtemp'];
      $sdhum = $_POST['sdhum'];
      $sdintensity = $_POST['sdintensity'];
      $sdpres = $_POST['sdpres'];
       

    }

    echo " Date: $d     ";
    echo "     Time: $t <BR>";
?>
<div class="row">
  <div class="column">
    <h2>Temperature</h2>
    <p><?php echo "Mean: $mtemp °C";?><br><?php echo "Standard Deviation: $sdtemp °C";?></p>
  </div>
  <div class="column">
    <h2>Humidity</h2>
    <p><?php echo "Mean: $mhum g.m-3";?><br><?php echo "Standard Deviation: $sdhum g.m-3";?></p>
  </div>
  <div class="column">
    <h2>Intensity</h2>
    <p><?php echo "Mean: $mintensity cd";?><br><?php echo "Standard Deviation: $sdintensity cd";?></p>
  </div>
  <div class="column">
    <h2>Absolute Pressure</h2>
    <p><?php echo "Mean: $mpres Pa";?><br><?php echo "Standard Deviation: $sdpres Pa";?></p>
  </div>
</div>


</body>
</html>