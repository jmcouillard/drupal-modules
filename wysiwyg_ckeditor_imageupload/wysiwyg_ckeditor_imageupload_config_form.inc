<?php


/**
 * Form function, called by drupal_get_form()
 * in the admin page.
 */
function wysiwyg_ckeditor_imageupload_config_form($form, &$form_state) {

	// Add content type selection
	$form['wysiwyg_ckeditor_imageupload_disable_advanced'] = array(
		'#type' => 'checkbox',
		'#title' => t('Hide Advanced tab'),
		'#default_value' => variable_get('wysiwyg_ckeditor_imageupload_disable_advanced', FALSE),
		'#description' => t('Hide the Advanced tab in the CKEditor image dialog.'),
		'#required' => TRUE,
	);

	return system_settings_form($form);
}

?>