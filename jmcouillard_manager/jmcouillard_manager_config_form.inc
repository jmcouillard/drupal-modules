<?php

/**
* Form function, called by drupal_get_form()
* in the admin page.
*/
function jmcouillard_manager_config_form($form, &$form_state) {

  // Get formatted content types
  $types = _node_types_build()->types;
  $processedTypes = array();  
  foreach($types as $type) $processedTypes[$type->type] = $type->name;

  // Get integer fields
  $intFields = field_read_fields(array('type'=>"number_integer"));
  $processedIntFields = array();
  foreach($intFields as $key=>$field) $processedIntFields[$key] = $field['field_name'];
  
  // Get image fields
  $imageFields = field_read_fields(array('type'=>"image"));
  $processedImageFields = array();
  foreach($imageFields as $key=>$field) $processedImageFields[$key] = $field['field_name'];
  
  // Get styles
  $styles = image_styles();
  $processedStyles = array();
  foreach($styles as $key=>$field) $processedStyles[$key] = $field['name'];
  

  // Add content type selection
  $form['jmcouillard_manager_type'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Managed content type'),
    '#default_value' => variable_get('jmcouillard_manager_type'),
    '#options' => $processedTypes,
    '#description' => t('Only the selected content type will be protected.'),
    '#required' => TRUE,
  );

  // Add weight field selection
  $form['jmcouillard_manager_weight_field'] = array(
    '#type' => 'radios',
    '#title' => t('Weight field'),
    '#default_value' => variable_get('jmcouillard_manager_weight_field'),
    '#options' => $processedIntFields,
    '#description' => t('Field used to order nodes (weight). Must be a integer field.'),
    '#required' => TRUE,
  );

  // Add image field selection
  $form['jmcouillard_manager_image_field'] = array(
    '#type' => 'radios',
    '#title' => t('Image field'),
    '#default_value' => variable_get('jmcouillard_manager_image_field'),
    '#options' => $processedImageFields,
    '#description' => t('Field used to display thumbnail. Must be an image field.'),
    '#required' => TRUE,
  );
  // Add image field selection
  $form['jmcouillard_manager_image_field_fallback'] = array(
    '#type' => 'radios',
    '#title' => t('Fallback image field'),
    '#default_value' => variable_get('jmcouillard_manager_image_field_fallback'),
    '#options' => $processedImageFields,
    '#description' => t('Field used to display thumbnail when the first one is unavailable. Must be an image field.'),
    '#required' => FALSE,
  );

  // Add thumbnail style selection
  $form['jmcouillard_manager_thumb_style'] = array(
    '#type' => 'radios',
    '#title' => t('Image style'),
    '#default_value' => variable_get('jmcouillard_manager_thumb_style'),
    '#options' => $processedStyles,
    '#description' => t('Image style used to display thumbnail.'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}


?>