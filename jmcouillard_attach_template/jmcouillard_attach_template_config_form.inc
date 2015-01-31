<?php

/**
 * Form function, called by drupal_get_form()
 * in the admin page.
 */
function jmcouillard_attach_template_config_form($form, &$form_state) {

	// Add content type selection
	$form['jmcouillard_attach_template_view_modes'] = array(
		'#type' => 'textfield',
		'#title' => t('Attach templates to the following view modes'),
		'#default_value' => variable_get('jmcouillard_attach_template_view_modes', 'full'),
		'#description' => t('Machine name separated by spaces.'),
		'#required' => TRUE,
	);

	return system_settings_form($form);
}
?>