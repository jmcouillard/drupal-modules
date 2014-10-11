<?php

/**
 * Form function, called by drupal_get_form()
 * in the admin page.
 */
function jmcouillard_attach_template_config_form($form, &$form_state) {

	// Get formatted content types
	$types = _node_types_build()->types;
	$processedTypes = array();
	foreach ($types as $type) {
		$processedTypes[$type->type] = $type->name;
	}

	// Add content type selection
	$form['jmcouillard_attach_template_type'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Allow template on following types content type'),
		'#default_value' => variable_get('jmcouillard_attach_template_type', array('basic_page')),
		'#options' => $processedTypes,
		'#description' => t('The template selection tool will only appear on the selected content types.'),
	);

	return system_settings_form($form);
}
?>