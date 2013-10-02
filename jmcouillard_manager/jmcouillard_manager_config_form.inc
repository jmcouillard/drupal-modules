<?php


/**
 * Form function, called by drupal_get_form()
 * in the admin page.
 */
function jmcouillard_manager_config_form($form, &$form_state) {

	// Get formatted content types
	$types =  _node_types_build()->types;
	$processedTypes = array();
	foreach ($types as $type)
		$processedTypes[$type->type] = $type->name;

	// Get all fields
	$allFields = field_read_fields();
	$processedAllFields = array();
	foreach ($allFields as $key => $field)
		$processedAllFields[$key] = $field['field_name'];	

	// Get integer fields
	$intFields = field_read_fields(array('type' => "number_integer"));
	$processedIntFields = array();
	foreach ($intFields as $key => $field)
		$processedIntFields[$key] = $field['field_name'];

	// Get image fields
	$imageFields = field_read_fields(array('type' => "image"));
	$processedImageFields = array();
	foreach ($imageFields as $key => $field)
		$processedImageFields[$key] = $field['field_name'];

	// Get term fields
	$taxonomyFields = field_read_fields(array('type' => "taxonomy_term_reference"));
	$processedTaxonomyFields = array();
	foreach ($taxonomyFields as $key => $field)
		$processedTaxonomyFields[$key] = $field['field_name'];

	// Get styles
	$styles = image_styles();
	$processedStyles = array();
	foreach ($styles as $key => $field)
		$processedStyles[$key] = $field['name'];

	// Get voacubularies
	$voc = taxonomy_get_vocabularies();
	$processedVoc = array();
	foreach ($voc as $key => $item)
		$processedVoc[$item->machine_name] = $item->name;

	// Get forms listing
	$modulePath = drupal_get_path("module", "jmcouillard_manager");
	$forms = file_scan_directory($modulePath . "/custom_forms", '/.*\.inc$/');

	// Add content type selection
	$form['jmcouillard_manager_type'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Managed content type'),
		'#default_value' => variable_get('jmcouillard_manager_type', array()),
		'#options' => $processedTypes,
		'#description' => t('Only the selected content type will be protected.'),
		'#required' => TRUE,
	);

	if(count($voc) > 0) {
		// Add vocabularies selection
		$form['jmcouillard_manager_voc'] = array(
			'#type' => 'checkboxes',
			'#title' => t('Managed vocabularies'),
			'#default_value' => variable_get('jmcouillard_manager_voc', array()),
			'#options' => $processedVoc,
			'#description' => t('Only the selected content type will be protected.'),
			'#required' => FALSE,
		);
	}

	// Add weight field selection
	$form['jmcouillard_manager_weight_field'] = array(
		'#type' => 'radios',
		'#title' => t('Weight field'),
		'#default_value' => variable_get('jmcouillard_manager_weight_field'),
		'#options' => $processedIntFields,
		'#description' => t('Field used to order nodes (weight). Must be a integer field.'),
		'#required' => TRUE,
	);

	// Add thumbnail style selection
	$form['jmcouillard_manager_default'] = array(
		'#type' => 'textfield',
		'#title' => t('Default page'),
		'#default_value' => variable_get('jmcouillard_manager_default'),
		'#description' => t('Default path to go when not specified.'),
		'#required' => FALSE,
	);
	
	// Add image fieldset
	$form['jmcouillard_manager_image'] = array(
		'#type' => 'fieldset',
		'#title' => t('Image'),
		'#collapsible' => TRUE,
	);

	// Add image field selection
	$form['jmcouillard_manager_image']['jmcouillard_manager_image_field'] = array(
		'#type' => 'radios',
		'#title' => t('Image field'),
		'#default_value' => variable_get('jmcouillard_manager_image_field'),
		'#options' => $processedImageFields,
		'#description' => t('Field used to display thumbnail. Must be an image field.'),
		'#required' => TRUE,
	);
	
	// Add image field selection
	$form['jmcouillard_manager_image']['jmcouillard_manager_image_field_fallback'] = array(
		'#type' => 'radios',
		'#title' => t('Fallback image field'),
		'#default_value' => variable_get('jmcouillard_manager_image_field_fallback'),
		'#options' => $processedImageFields,
		'#description' => t('Field used to display thumbnail when the first one is unavailable. Must be an image field.'),
		'#required' => FALSE,
	);

	// Add thumbnail style selection
	$form['jmcouillard_manager_image']['jmcouillard_manager_thumb_style'] = array(
		'#type' => 'select',
		'#title' => t('Image style'),
		'#default_value' => variable_get('jmcouillard_manager_thumb_style'),
		'#options' => $processedStyles,
		'#description' => t('Image style used to display thumbnail.'),
		'#required' => TRUE,
	);

	if(count($voc) > 0) {

		// Add subcategory fieldset
		$form['jmcouillard_manager_subcategory'] = array(
			'#type' => 'fieldset',
			'#title' => t('Subcategory'),
			'#description' => t('Group items by a terms (ex.: categories, tags)'),
			'#collapsible' => TRUE,
		);

		// Add subcategory vocabulary selection
		$form['jmcouillard_manager_subcategory']['jmcouillard_manager_subcategory_vocabulary'] = array(
			'#type' => 'radios',
			'#title' => t('Subcategory vocabulary'),
			'#default_value' => variable_get('jmcouillard_manager_subcategory_vocabulary', array()),
			'#options' => array(-1 => t("(none)")) + $processedVoc,
			'#description' => t('Vocabulary to use as subcategories'),
			'#required' => TRUE,
		);

		// Add subcategory field selection
		$form['jmcouillard_manager_subcategory']['jmcouillard_manager_subcategory_field'] = array(
			'#type' => 'select',
			'#title' => t('Subcategory field'),
			'#default_value' => variable_get('jmcouillard_manager_subcategory_field'),
			'#options' => $processedTaxonomyFields,
			'#description' => t('The module will look into this field to filter the nodes by subcategories.'),
			'#empty_value' => -1,
			'#required' => FALSE,
		);

	}

	if(count($forms) > 0) {

		// Add subcategory fieldset
		$form['jmcouillard_manager_form'] = array(
			'#type' => 'fieldset',
			'#title' => t('Custom forms'),
			'#description' => t('Custom forms must be placed in the <em>custom_forms</em> folder of this module.'),
			'#collapsible' => TRUE,
		);

		foreach ($forms as $key => $formItem) {

			$formId = $formItem->name;
			$formFilename = $formItem->filename;

			// Add subcategory field selection
			$form['jmcouillard_manager_form']['jmcouillard_manager_form_' . $formId] = array(
				'#type' => 'select',
				'#title' => $formFilename,
				'#default_value' => variable_get('jmcouillard_manager_form_' . $formId),
				'#options' => array_merge($processedTypes, $processedVoc),
				'#description' => t('Associate a custom fom to a content type or taxonomy vocabulary.'),
				'#empty_value' => -1,
				'#required' => FALSE,
			);
		}
	}


	// Add extra_columns fieldset
	$form['jmcouillard_manager_extra_columns'] = array(
		'#type' => 'fieldset',
		'#title' => t('Extra columns'),
		'#description' => t('Slected fields will be shown in the table when availalable.'),
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);

	// Add extra_columns field selection
	$form['jmcouillard_manager_extra_columns']['jmcouillard_manager_extra_columns'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Extra columns'),
		'#default_value' => variable_get('jmcouillard_manager_extra_columns'),
		'#options' => $processedAllFields,
		'#description' => t('The module will look into this field to filter the nodes by subcategories.'),
		'#empty_value' => -1,
		'#required' => FALSE,
	);

	return system_settings_form($form);
}
?>