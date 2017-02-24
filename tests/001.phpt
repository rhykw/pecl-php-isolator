--TEST--
Check for isolator presence
--SKIPIF--
<?php if (!extension_loaded("isolator")) print "skip"; ?>
--FILE--
<?php 
echo "isolator extension is available";
?>
--EXPECT--
isolator extension is available
