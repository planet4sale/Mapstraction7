<?php

/**
 * @file
 * Contains APIs specific callbacks for settings forms and rendering.
 */

/**
 * A basic rendering callback that imports a JavaScript file using an API key.
 */
function mapstraction_basic_render($settings, $api_id) {
  $apis = mapstraction_apis(TRUE);
  $api = $apis[$api_id];
  
  // Find %key flag in 'src'
  $src = strtr($api['src'], array('%key' => variable_get('mapstraction_api_'. $api_id, '')));
  return '<script type="text/javascript" src="' . $src . '"></script>';
}

function mapstraction_yahoo_render($settings) {
  $src = 'http://api.maps.yahoo.com/ajaxymap?v=3.0&appid=' . variable_get('mapstraction_api_'. $api_id, '');
  return '<script type="text/javascript" src="' . $src . '"></script>';
}

function mapstraction_microsoft_render() {
  $src = 'http://dev.virtualearth.net/mapcontrol/mapcontrol.ashx?v=6';
  return '<script type="text/javascript" src="' . $src . '"></script>';
}

function mapstraction_freeearth_render() {
  $src = 'http://freeearth.poly9.com/api.js';
  return '<script type="text/javascript" src="' . $src . '"></script>';
}

function mapstraction_cloudmade_settings_form($settings) {
  $form = array();
  
  $form['style_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Map Style ID'),
    '#size' => 40,
    '#maxlength' => 255,
    '#default_value' => $settings['style_id'],
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array(
      'edit-style-options-api' => array('cloudmade'),
    ),
  );
  
  return $form;
}

function mapstraction_cloudmade_render($settings, $api_id, $id) {
  $output = mapstraction_openlayers_render($settings);
  $src = base_path() . drupal_get_path('module', 'mapstraction') . '/cloudmade.js';
  $output .= "<script type='text/javascript' src='$src'></script>\n";
  $js = '
      $(document).ready(function() {
        var cloudmade = new OpenLayers.Layer.CloudMade("CloudMade", {
          key: "' . variable_get('mapstraction_api_'. $api_id, '') . '",
          styleId: ' . $settings['style_id'] . ',
        });
        Drupal.mapstraction["' . $id . '"].maps["openlayers"].addLayer(cloudmade);
        Drupal.mapstraction["' . $id . '"].maps["openlayers"].setBaseLayer(cloudmade);
      });';

  drupal_add_js($js, 'inline', 'footer');
  
  return $output;
}

function mapstraction_openlayers_render($settings) {
  $src = 'http://openlayers.org/api/OpenLayers.js';
  return "<script type='text/javascript' src='$src'></script>\n";
}