<?php

function jmcouillard_manager_custom_form_get_fields() {

	// Common fields
	return array(
		"common" => array(
			"field_category",
			"field_area",
			"field_years",
			"field_location",
			"field_area",
			"field_years",
			"field_awards",
			"field_client",
			"field_image",
			"field_images"
		),
		"translated" => array(
			"field_subtitle",
			"field_quote",
			"field_quote_author",
			"body",
		)
	);

}

function jmcouillard_manager_custom_form_validate($form, &$form_state) {
	// Validation logic.
}

function jmcouillard_manager_custom_form_submit($form, &$form_state) {

	global $user;

	//Include pathauto module
	module_load_include('inc', 'pathauto');

	// Get langauges and fields
	$languages = language_list();
	$fields = jmcouillard_manager_custom_form_get_fields();

	if(isset($form_state['values'])) {

		$values = $form_state['values'];
		$editing = FALSE;

		if(isset($values["nid"])) {
			$node = node_load($values["nid"]);
			$node->title = $values["title"];
			$editing = TRUE;

		} else {
			// New node basic values
			$node = new stdClass();
			$node->type = 'project';
			$node->title = $values["title"];
			$node->language = 'und';
			$node->created = strtotime("now");
			$node->changed = strtotime("now");
			$node->uid = $user->uid;
			$node->status = 1;
			$node->comment = 0;
			$node->promote = 0;
			$node->moderate = 0;
			$node->sticky = 0;
			$editing = FALSE;
		}

		// Common fields
		foreach ($fields["common"] as $field) {
			$node->{$field} = clean_up_field($values[$field]);
		}

		// Language relative fields
		foreach ($fields["translated"] as $field) {
			$node->{$field} = clean_up_field($values[$field], array_keys($languages));			
		}

		// Generate a translation (a little hackish for now...)
		$handler = entity_translation_get_handler('node', $node);
		$translation = array(
			'translate' => 0,
			'status' => 1,
			'language' => "fr",
			'source' => "en",
		);
		$handler->setTranslation($translation, $node);
		
		// Save node
		node_save($node);

		// Update aliases
		foreach ($languages as $code => $lang) {
			pathauto_create_alias("node", "update", "node/" . $node->nid, array('node' => $node), $node->type, $code);
		}
		
		// Clear cache
		cache_clear_all('*', 'cache_page', TRUE);
		cache_clear_all('*', 'cache_path', TRUE);

		if($editing)
			drupal_set_message("Changes have been made.");
		else
			drupal_set_message("New product added successfully.");

	} else {

		drupal_set_message("Datas were missing.", "error");
	}
}

function jmcouillard_manager_custom_form($form, &$form_state, $nid = NULL) {

	// Get langauges and fields
	$languages = language_list();
	$fields = jmcouillard_manager_custom_form_get_fields();

	// Create form object and base structure
	$form = array(
		'#prefix' => '<div class="jalo-cms">',
		'#suffix' => '</div>',
		'#theme' => 'jalo_manager_table',
		'actions' => array("#weight" => 1001, ),
	);

	// If node is not set, create an empty object
	if(!isset($nid)) {
		$node = new stdClass();
		$node->type = 'project';
		$node->language = 'und';
		$node->status = 1;
		drupal_set_title(t('Add a new project'));
	} else {
		$node = node_load($nid);
		drupal_set_title('Edit ' . $node->title);
	}

	// Title
	$form['common']['title'] = array(
		'#title' => 'Title',
		'#name' => 'title',
		'#default_value' => isset($node->title) ? $node->title : "",
		'#type' => 'textfield',
		'#required' => TRUE,
		'#weight' => 0,
	);

	// Published
	$form['common']['published'] = array(
		'#type' => 'checkbox',
		'#default_value' => $node->status,
		'#title' => t('Published'),
		'#weight' => 1,
	);

	// Nid
	$form['nid'] = array(
		'#value' => isset($node->nid) ? $node->nid : NULL,
		'#type' => 'hidden',
	);

	// Common fields
	foreach ($fields["common"] as $field) {
		_jmcouillard_manager_attach_form($field, 'node', 'project', $node, $form, $form_state, LANGUAGE_NONE);
	}

	// Language relative fields
	foreach ($languages as $code => $lang) {

		$form[$code] = array(
			"#type" => "fieldset",
			"#title" => $lang->name,
			"#attributes" => array("class" => array("lang")),
			"#weight" => 1000,
			"#prefix" => '<div style="width:50%;float:left;">',
			"#suffix" => '</div>',
			"#tree" => TRUE,
		);

		foreach ($fields["translated"] as $field) {
			_jmcouillard_manager_attach_form($field, 'node', 'project', $node, $form[$code], $form_state, $code);
		}
	}

	// Submit
	$form['actions']['submit'] = array(
		'#name' => 'jalo_manager_product_form',
		'#type' => 'submit',
		'#weight' => 1001,
		'#value' => t('Submit'),
	);

	return $form;
}

function clean_up_field(&$field, $languages = array(LANGUAGE_NONE)) {

	foreach ($languages as $lang) {
		if(!isset($field[$lang])) {
			// Handle empty fields
			$field = NULL;
		} else if(isset($field[$lang]["value"]) && $field[$lang]["value"] == "") {	
			// Handle empty translated fields		
			$field = NULL;
		} else {	
			// Handle empty multiple items fields	
			foreach ($field[$lang] as $key => $item) {				
				if(isset($item["fid"]) && $item["fid"] <= 0) {
					$field[$lang][$key] = NULL;
					unset($field[$lang][$key]);
				} else if(isset($field[$lang][0]["value"]) && $field[$lang][0]["value"] == "") {
					$field[$lang][$key] = NULL;
					unset($field[$lang][$key]);
				}
			}
			usort($field[$lang], 'sort_objects_by_weight');
		}
	}	

	return $field;
}

function sort_objects_by_weight($a, $b) {
	if($a["_weight"] == $b["_weight"]) {
		return 0;
	}
	return ($a["_weight"] < $b["_weight"]) ? -1 : 1;
}
?>