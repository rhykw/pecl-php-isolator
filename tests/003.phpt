--TEST--
Check for isolator_chroot presence
--SKIPIF--
<?php if (!extension_loaded("isolator")) print "skip"; ?>
--FILE--
<?php 
if (function_exists("isolator_chroot")){
  echo "function isolator_chroot exists";
}else{
  echo "function isolator_chroot does not exist";
}
?>
--EXPECT--
function isolator_chroot exists
