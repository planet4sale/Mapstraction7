<?php

/**
 * @file
 * Views integration for Mapstraction module.
 */

/**
 * Implements hook_views_plugins().
 */
function mapstraction_views_plugins() {
  return array(
    'module' => 'mapstraction',
    'style' => array(
      'mapstraction' => array(
        'title' => 'Mapstraction',
        'help' => 'Displays nodes as a Mapstraction map.',
     //    'theme' => 'mapstraction-map',
        'handler' => 'mapstraction_style_map',
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'uses row plugin' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
}