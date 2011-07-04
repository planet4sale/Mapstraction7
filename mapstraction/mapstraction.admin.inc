<?php
/**
 * @file
 * Administration page callbacks for the mapstraction module.
*/

function mapstraction_admin_settings(){
  $form = array();
  $form['mapstraction_keys'] = array(
    '#type' => 'fieldset',
    '#title' => t('API Keys'),
    '#description' => t('Provide API keys for any of the providers below you will be using.'),
    '#collapsible' => FALSE,
  );
  
  foreach(mapstraction_apis(TRUE) as $key => $api){
    if (!empty($api['has_key'])) {
      $form['mapstraction_keys']['mapstraction_api_'. $key] = array(
        '#type' => 'textfield',
        '#title' => $api['title'],
        '#default_value' => variable_get('mapstraction_api_'. $key, NULL)
      );
    }
  }

  return system_settings_form($form);
}