<?php
/* @file
 * mapstraction.map.tpl.php
 *
 * this file is all about getting theme preprocess functionality.
 *
 * script code
 *   $api_script
 * div attributes
 *   $width
 *   $height
 *   $map_id
 */
?>
<?php print $api_script; ?>
<div id="<?php print $map_id; ?>" style="width: <?php print $width; ?>; height: <?php print $height; ?>;"></div>