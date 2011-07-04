<?php

/**
 * @file
 * Defines a style plugin that will display points on a map from a number of
 * different providers.
 */
class mapstraction_style_map extends views_plugin_style {

  /**
   * Views options definitions
   *
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['api'] = array('default' => array());
    $options['width'] = array('default' => 'auto');
    $options['height'] = array('default' => '400px');

    $options['initial_point']['contains'] = array(
      'auto' => array('default' => TRUE),
      'latitude' => array('default' => ''),
      'longitude' => array('default' => ''),
      'zoom' => array('default' => 10),
      'zoom_single' => array('default' => 5),
    );
    $options['controls'] = array('default' => array());
    $options['behaviours'] = array('default' => array());
    $options['overlays'] = array('default' => array('markers' => 'markers'));
    $options['zoom_control'] = array('default' => array());
    $options['default_maptype'] = array('default' => array());
    $options['fields']['contains'] = array(
      'latitude' => array('default' => ''),
      'longitude' => array('default' => ''),
      'class' => array('default' => ''),
    );

    return $options;
  }

  /**
   * Return a form containing the mapping options for the view.
   *
   * @param string $form
   * @param string $form_state
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['api'] = array(
      '#type' => 'select',
      '#title' => t('Mapping API'),
      '#description' => t('Select the Mapstraction API to use for this view. Make sure you have specfied an !api where needed.', array('!api' => l('API key', 'admin/settings/mapstraction'))),
      '#options' => mapstraction_apis(),
      '#default_value' => $this->options['api'],
    );

    foreach (mapstraction_apis(TRUE) as $key => $api) {
      module_load_include('inc', 'mapstraction', 'mapstraction.apis');
      if (isset($api['settings form']) && function_exists($api['settings form'])) {
        $form['api_settings'][$key] = $api['settings form']($this->options['api_settings'][$key], $key);
      }
    }

    $form['width'] = array(
      '#type' => 'textfield',
      '#title' => t('Width'),
      '#size' => 10,
      '#maxlength' => 255,
      '#default_value' => $this->options['width'],
    );

    $form['height'] = array(
      '#type' => 'textfield',
      '#title' => t('Height'),
      '#size' => 10,
      '#maxlength' => 255,
      '#default_value' => $this->options['height'],
    );

    $form['initial_point'] = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#title' => t('Initial point'),
    );

    $form['initial_point']['auto'] = array(
      '#type' => 'checkbox',
      '#title' => t('Auto detect initial point'),
      '#description' => t('If checked, the map will auto detect initial coordinates and zoom level based on all markers on the map. Otherwise you can specify these values by yourself. You can also specify the zoom level to use for a single result.'),
      '#default_value' => $this->options['initial_point']['auto'],
    );

    $form['initial_point']['latitude'] = array(
      '#type' => 'textfield',
      '#title' => t('Latitude'),
      '#size' => 40,
      '#maxlength' => 255,
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array(
        'edit-style-options-initial-point-auto' => array(0)
      ),
      '#default_value' => $this->options['initial_point']['latitude'],
    );

    $form['initial_point']['longitude'] = array(
      '#type' => 'textfield',
      '#title' => t('Longitude'),
      '#size' => 40,
      '#maxlength' => 255,
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array(
        'edit-style-options-initial-point-auto' => array(0)
      ),
      '#default_value' => $this->options['initial_point']['longitude'],
    );

    $form['initial_point']['zoom'] = array(
      '#type' => 'textfield',
      '#title' => t('Zoom level'),
      '#size' => 2,
      '#maxlength' => 2,
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array(
        'edit-style-options-initial-point-auto' => array(0)
      ),
      '#default_value' => $this->options['initial_point']['zoom'],
    );
    
    $form['initial_point']['zoom_single'] = array(
      '#type' => 'textfield',
      '#title' => t('Single result zoom level'),
      '#size' => 2,
      '#maxlength' => 2,
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array(
        'edit-style-options-initial-point-auto' => array(1)
      ),
      '#default_value' => $this->options['initial_point']['zoom_single'],
    );    

    $form['behaviours'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Behaviours'),
      '#options' => array(
        'hover' => t('Show info bubble on mouse hover'),
        'declutter' => t('Declutter points')
      ),
      '#default_value' => $this->options['behaviours'],
    );

    $form['overlays'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Display following overlay types'),
      '#options' => array(
        'markers' => t('Markers'),
        'polylines' => t('Polyines'),
      ),
      '#default_value' => $this->options['overlays'],
    );

    $form['controls'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Controls'),
      '#description' => t('Select which controls should be displayed on the map.'),
      '#options' => array(
        'pan' => t('Pan'),
        'overview' => t('Overview'),
        'scale' => t('Scale'),
        'map_type' => t('Map type')),
      '#default_value' => $this->options['controls'],
    );

    $form['zoom_control'] = array(
      '#type' => 'select',
      '#title' => t('Zoom Control'),
      '#options' => array(
        0 => t('None'),
        'large' => t('Large'),
        'small' => t('Small'),
      ),
      '#default_value' => $this->options['controls']['zoom'],
      '#description' => t('Select an optional zoom control.'),
    );

    $form['default_maptype'] = array(
      '#type' => 'select',
      '#title' => t('Default Map Type'),
      '#description' => t('Select which map type you would like to use as default: Standard Map, Satellite, or Hybrid'),
      '#options' => array(
        'mxn.Mapstraction.ROAD' => t('Map'),
        'mxn.Mapstraction.SATELLITE' => t('Satellite'),
        'mxn.Mapstraction.HYBRID' => t('Hybrid'),
        'mxn.Mapstraction.PHYSICAL' => t('Physical'),
      ),
      '#default_value' => $this->options['controls']['default_maptype'],
    );

    $handlers = $this->display->handler->get_handlers('field');
    if (empty($handlers)) {
      $form['error_markup'] = array(
        '#value' => t('You need at least one field before you can configure your field settings'),
        '#prefix' => '<div class="error form-item description">',
        '#suffix' => '</div>',
      );
    }
    else {
      $field_names = array('' => '--');
      foreach ($handlers as $field => $handler) {
        if ($label = $handler->label()) {
          $field_names[$field] = $label;
        }
        else {
          $field_names[$field] = $handler->ui_name();
        }
      }
      $field_options = array(
        'latitude' => t('Latitude'),
        'longitude' => t('Longitude'),
        'icon' => t('Icon URL'),
      );
      $form['fields'] = array(
        '#type' => 'fieldset',
        '#title' => 'Field usage',
        '#description' => t('Select the fields that contain the latitude, longitude, and icon of each point. You can also select one or more attributes to associate with each marker.'),
      );
      foreach ($field_options as $k => $v) {
        $form['fields'][$k] = array(
          '#type' => 'select',
          '#title' => $v,
          '#options' => $field_names,
          '#default_value' => $this->options['fields'][$k],
          '#required' => ($k == 'icon' ? FALSE : TRUE),
        );
      }

      $form['fields']['attributes'] = array(
        '#title' => 'Attributes',
        '#type' => 'select',
        '#multiple' => TRUE,
        '#options' => $field_names,
        '#default_value' => $this->options['fields']['attributes'],
      );
    }
  }

  /**
   * Options form submit handler. Map zoom and maptype to a the controls group in the
   * form tree.
   *
   * @param string $form
   * @param string $form_state
   */
  function options_submit($form, &$form_state) {
    $form_state['values']['style_options']['controls']['zoom'] = $form_state['values']['style_options']['zoom_control'];
    unset($form_state['values']['style_options']['zoom_control']);

    $form_state['values']['style_options']['controls']['default_maptype'] = $form_state['values']['style_options']['default_maptype'];
    unset($form_state['values']['style_options']['default_maptype']);
  }

  /**
   * Validate the options form.
   */
  function validate() {
    $errors = parent::validate();

    // Validate the field style for location.module's lat/lon fields. They
    // must be set to decimal degress.
    $fields = $this->display->handler->get_handlers('field');
    $field_array = array($fields[$this->options['fields']['latitude']],
    $fields[$this->options['fields']['longitude']]);

    foreach ($field_array as $field) {
      if ($field->options['table'] == 'location' && $field->options['style'] != 'dd') {
        $errors[] = t('The display style for @field must be set to decimal degrees.', array('@field' => $field->options['label']));
      }
    }

    return $errors;
  }

  /**
   * Return output that will render the view as a map
   */
  function render() {
    if (!empty($this->view->preview) && $this->view->preview && $this->view->live_preview) {  
      return '<div class="messages error">Preview is disabled for the Mapstraction style plugin.</div>' . parent::render();
    }
    else {
      // Would like to use parent::render() here but there seems to be problem
      // with the way the theme function is handled. Copying this from
      // views_plugin_style.inc for now.
      if ($this->uses_row_plugin() && empty($this->row_plugin)) {
        vpr('views_plugin_style_default: Missing row plugin');
        return;
      }
      
      // Group the rows according to the grouping field, if specified.
      $sets = $this->render_grouping($this->view->result, $this->options['grouping']);
      
      $point_groups = array();
      foreach ($sets as $title => $records) {
        $point_groups[] = array(
          'title' => $title,
          'markers' => $this->map_points($records),
        );
      }

      return mapstraction_render_map(
        $this->view->name . '-' . $this->view->current_display,
        $this->view->style_plugin->options['api'],
        $this->options,
        $point_groups
      );
    }
  }

  /**
   * Convert views results into array of points to render on the map
   */
  protected function map_points($rows = array()) {
    $points = array();
    $uses_row_plugin = $this->uses_row_plugin();
    foreach($rows as $row_index => $row) {
      //an essential to use row_plugin->render()
      $this->view->row_index = $row_index;
      $point = self::map_point($this->rendered_fields[$row_index], $this->options['fields']);

      // the content of the info window as a rendered row
      if ($uses_row_plugin) {
        $point['text'] = $this->row_plugin->render($row);
      }

      // add the row's nid where applicable
      if ($row->nid) {
        $point['nid'] = $row->nid;
      }

      $points[] = $point;
    }
    unset($this->view->row_index);

    return $points;
  }

  /*
   * Translate given fields of a single row into point data
   */
  static function map_point($row, $field_map) {
    $point = array();

    foreach ($field_map as $key => $value) {
      // handle fields with more than one value
      if (is_array($value)){
        foreach($value as $item){
          if (!empty($item)) {
            $values[$item] = $row[$item];
          }
        }
        $point[$key] = $values;
      }
      elseif ($key == 'icon') {
        // special case for icons as we want to get the image size
        $point[$key] = self::get_icon_field_value($row[$value]);
      }
      else {
        $point[$key] = $row[$value];
      }
    }

    return $point;
  }

  /*
   * Special case for icons as we want to get the image size
   */
  private static function get_icon_field_value($path) {
    $field = array();
    if (!empty($path)){
      if (is_file($path)){
        list($field['width'], $field['height']) = getimagesize($path);
        $path = (url($path) == $path) ? $path : (base_path() . $path);
      }
      $field['url'] = $path;
    }
    return $field;
  }
}
