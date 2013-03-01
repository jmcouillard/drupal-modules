<?php

function jmcouillard_manager_form_validate($form, &$form_state) {
	// Validation logic.
}

function jmcouillard_manager_form_submit($form, &$form_state) {

	global $types;

	// Define dynamic fields
	$definedWeightField = variable_get('jmcouillard_manager_weight_field', "");
	// $definedImageField = variable_get('jmcouillard_manager_image_field', "");
	// $definedImageStyle = variable_get('jmcouillard_manager_thumb_style', "");

	// For each item form element
	if (isset($form_state['values']['items'])) {

		foreach ($form_state['values']['items'] as $nid => $item) {

			$node = node_load($nid);
			$node -> title = $item['title'];
			$node -> {$definedWeightField}['und'][0]['value'] = $item['weight'];

			node_save($node);
		}

	}

	if (isset($form_state['values']['variable'])) {

		$varname = $types[$form_state['values']['type']]["variable"]["name"];
		variable_set($varname, $form_state['values']['variable']['value']);
	}

	drupal_set_message("Changes have been made.");
}

function theme_jmcouillard_manager_table($vars) {

	global $types;

	$form = $vars['element'];
	$rows = array();
	$attributes = array('id' => array('jmcouillard_manager_form_table'));
	$type = $form["global"]["type"]["#value"];
	// $type = isset($types[$type]) ? $type : "front_slider";
	$output = '';

	// Render variable form children first
	if (isset($form["global"]["variable"])) {
		$output .= drupal_render($form["global"]["variable"]);
		unset($form["global"]["variable"]);
	}

	// Create table header <th> : must be same count as td, otherwise tabledrag won't work.
	$header = array(
		t('Preview'),
		t('Title'),
		t('Edit')
	);

	// Deletable table header
	if ($types[$type]["deletable"]) {
		$header[] = t('Delete');
	}

	// Sortable table header
	if ($types[$type]["sortable"]) {
		$header[] = t('Weight');
	}

	// Only do table business if there are items
	if(count(element_children($form['items']))>0) {

		// For each item form element
		foreach (element_children($form['items']) as $key) {

			// Gete element to make it easier to access
			$element = $form['items'][$key];

			// Create row if element has an id
			$rows[$key] = array('data' => array(
					array('data' => $element['title']["#extra_data"]['img']),
					array('data' => $element['title']),
					array('data' => $element['title']["#extra_data"]['edit_link']),
				), );

			// Deletable field
			if ($types[$type]["deletable"]) {
				$rows[$key]["data"][] = array('data' => $element['title']["#extra_data"]['delete_link']);
			}

			// Sortable field
			if ($types[$type]["sortable"]) {
				$rows[$key]["data"][] = array('data' => $element['weight']);
				$rows[$key]['class'] = array('draggable');
			}
		}

		// Render table
		$output .= theme('table', array(
			'header' => $header,
			'rows' => $rows,
			'tree' => TRUE,
			'attributes' => $attributes
		));
		
		unset($form['items']);

		// Add drag js
		if ($types[$type]["sortable"])
			drupal_add_tabledrag('jmcouillard_manager_form_table', 'order', 'sibling', 'retouche-manager-weight');
	}

	// Render other form items
	$output .= drupal_render_children($form);

	// Render and return
	return $output;
}

function jmcouillard_manager_form($form, &$form_state, $typename = NULL) {

	//	$types = jmcouillard_manager_types();

	global $types;

	// Set default typename if not defined
	if(!isset($typename)) {
		$typename = array_shift(array_keys($types));
	}

	// Define dynamic fields
	$definedWeightField = variable_get('jmcouillard_manager_weight_field', "");
	$definedImageField = $types[$typename]["imageField"];
	$definedImageStyle = variable_get('jmcouillard_manager_thumb_style', "");

	// Load content type
	$type = node_type_load($typename);

	// Define module path
	$modulePath = drupal_get_path('module', 'jmcouillard_manager');

	// Add custom css
	drupal_add_css($modulePath  . "/jmcouillard_manager_form_table.css");

	$form = array(
		'#prefix' => '<div id="people">',
		'#suffix' => '</div>',
		'#theme' => 'jmcouillard_manager_table',
	);

	$form['items']['#tree'] = TRUE;
  
	if($types[$typename]["sortable"]) {
		$results = db_query("SELECT * FROM {node} AS n LEFT JOIN {field_data_".$definedWeightField."} AS w ON n.nid = w.entity_id WHERE n.type=:type ORDER BY " . $types[$typename]["order"], array(':type' => $typename));
	} else {
		$results = db_query("SELECT * FROM {node} AS n WHERE n.type=:type ORDER BY n.created DESC", array(':type' => $typename));
	}

	$count = 0;
	while ($row = $results -> fetch()) {

		if (!isset($row))
			break;

		// Load node
		$node = node_load($row -> nid);

		// Create data
		$data = array();
		$data['nid'] = $node -> nid;
		$data['title'] = $node -> title;
		$data['weight'] = isset($node -> {$definedWeightField}['und']) ? $node -> {$definedWeightField}['und'][0]['value'] : 0;
		$data['edit_link'] = l(t('edit'), 'node/' . $node -> nid . '/edit', array('query' => array('destination' => '/admin/content/manager/nodes/' . $typename)));
		$data['delete_link'] = l(t('delete'), 'node/' . $node -> nid . '/delete', array('query' => array('destination' => '/admin/content/manager/nodes/' . $typename)));

		// Define image url (for thumbnail)
		if (isset($node -> {$definedImageField}["und"])) {
			$data['img_url'] = image_style_url($definedImageStyle, $node -> {$definedImageField}["und"][0]["uri"]);
		} else if (isset($node -> {$definedImageField}["und"])) {
			$data['img_url'] = image_style_url($definedImageStyle, $node -> {$definedImageField}["und"][0]["uri"]);
		} else {
			$data['img_url'] = $modulePath . "/jmcouillard_manager_default.png";
		}

		// Define image uri
		$data['img'] = l(theme_image(array(
			"path" => $data['img_url'],
			"title" => $node -> title,
			"attributes" => array(
				"width" => 100,
				"height" => 100
			)
		)), ("node/" . $node -> nid), array("html" => TRUE));

		$field_weight = array( 
			'#type' => 'weight',
			'#default_value' => $data['weight'],
			'#delta' => 50,
			'#attributes' => array('class' => array('retouche-manager-weight')),
			'#extra_data' => $data,
		);

		$field_title = array(
			'#type' => 'textfield',
			'#default_value' => $data['title'],
			'#extra_data' => $data,
		);

		$form['items'][$data['nid']] = array(
			"weight" => $field_weight,
			"title" => $field_title
		);

		unset($row);
		$count ++;
	};

	// Create textfield text area
	if (isset($types[$typename]["variable"])) {
		$form['global']['variable'] = array(
			'#name' => 'variable',
			'#type' => 'text_format',
			'#format' => "html",
			'#title' => $types[$typename]["variable"]["readable"],
			'#default_value' => variable_get($types[$typename]["variable"]["name"], ""),
			"#weight" => -100,
		);

		global $useSmallCKEditor;
		$useSmallCKEditor = TRUE;
	}

	// Ceate type hidden
	$form['global']['type'] = array(
		'#name' => 'type',
		'#type' => 'hidden',
		'#value' => $typename
	);

	if($count >0){
		// Don't forget the submit button if there are some items
		$form['actions'] = array('#type' => 'actions');
		$form['actions']['submit'] = array(
			'#name' => 'submit',
			'#type' => 'submit',
			'#value' => t('Save changes'),
		);
	} else {
		// There is no items
		$form["#prefix"] =  t("There are no item.") . $form["#prefix"];

	}

	// $form['actions']['add'] = array(
	// '#name' => 'add',
	// '#type' => 'submit',
	// '#value' => t('Add new'),
	// '#submit' => array('_jmcouillard_manager_add_new'),
	// );

	return $form;
}

?>