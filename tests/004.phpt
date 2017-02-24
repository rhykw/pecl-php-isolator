--TEST--
Check for isolator_mount presence
--SKIPIF--
<?php if (!extension_loaded("isolator")) print "skip"; ?>
--FILE--
<?php 
if (function_exists("isolator_mount")){
  echo "function isolator_mount exists";
}else{
  echo "function isolator_mount does not exist";
}
?>
--EXPECT--
function isolator_mount exists
