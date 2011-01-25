#!/usr/bin/php
<?php

function printImageBytes($filename) {
  $im = imagecreatefrompng($filename);
  $width = imagesx($im);
  $height = imagesy($im);
  for ($y = 0; $y < $height; $y += 8) {
    for ($x = 0; $x < $width - 1; $x++) {
      $byte = 0;
      for ($bit = 0; $bit < 8; $bit++) {
        if (imagecolorat($im, $x, $y + $bit) == 0) {
          $byte += (1 << $bit);
        }
      }
      echo sprintf("0x%02X", $byte) . ",";
    }
    echo "\n";
  }
}

echo "static unsigned char __attribute__ ((progmem)) DigitsLarge[] = {\n";
for ($i = 0; $i < 10; $i++) {
  if ($i > 0) echo "\n";
  printImageBytes("l$i.png");
}
echo "};\n\n";

echo "static unsigned char __attribute__ ((progmem)) DigitsSmall[] = {\n";
for ($i = 0; $i < 10; $i++) {
  if ($i > 0) echo "\n";
  printImageBytes("s$i.png");
}
echo "};\n\n";

echo "static unsigned char __attribute__ ((progmem)) Weekdays[] = {\n";
for ($i = 0; $i < 7; $i++) {
  if ($i > 0) echo "\n";
  printImageBytes("d$i.png");
}
echo "};\n\n";

?>
