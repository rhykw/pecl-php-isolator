--TEST--
Check for isolator_unshare presence
--SKIPIF--
<?php if (!extension_loaded("isolator")) print "skip"; ?>
--FILE--
<?php 
if (function_exists("isolator_unshare")){
  echo "function isolator_unshare exists";
}else{
  echo "function isolator_unshare does not exist";
}
?>
--EXPECT--
function isolator_unshare exists
