<?php

header('Content-Type: application/json');

/* FILL WITH OWN DETAILS */
$api_key 	= "OPENWEATHERMAP_API_KEY";
$city 		= "OPENWEATHERMAP_CITY";
$url 		= "https://api.openweathermap.org/data/2.5/weather?q=$city&APPID=$api_key";
$json 		= file_get_contents($url);

$weather 	= json_decode($json);
$temp 		= $weather->main->temp;
$temp_min 	= $weather->main->temp_min;
$temp_max 	= $weather->main->temp_max;

$temp 		= $temp - 273.15;
$temp_min 	= $temp_min - 273.15;
$temp_max 	= $temp_max - 273.15;
$humidity 	= $weather->main->humidity;
$pressure 	= $weather->main->pressure;
$conditions = $weather->weather[0]->main;

$date 		= getdate();
$weekday 	= date("D");

$data = array(
	"temp" => round($temp),
    "temp_min" => round($temp_min),
    "temp_max" => round($temp_max),
    "humidity" => $humidity,
	"pressure" => $pressure,
	"conditions" => $conditions,
	"weekday" => $weekday,
    "day" => $date["mday"],
    "month" => $date["mon"],
    "year" => $date["year"],
	"hours" => $date["hours"],
	"minutes" => $date["minutes"],
	"seconds" => $date["seconds"]
);

echo json_encode($data);

?>