<?php

function jmcouillard_manager_form_validate($form, &$form_state) {
	// Validation logic.
}

function jmcouillard_manager_form_submit($form, &$form_state) {

	// Define dynamic fields
	$weightField = variable_get('jmcouillard_manager_weight_field', "");

	// For each item form element
	if (isset($form_state['values']['items'])) {

		foreach ($form_state['values']['items'] as $id => $item) {

			// Edit nodes
			if($item['type'] == "node") {

				$node = node_load($id);
				if(isset($item['title'])) $node->title = $item['title'];
				$node->{$weightField}['und'][0]['value'] = $item['weight'];

				node_save($node);

			// Edit terms
			} else if($item['type'] == "taxonomy_term") {

				$term = taxonomy_term_load($id);
				if(isset($item['title'])) $term->name = $item['title'];
				$term->weight = $item['weight'];

				taxonomy_term_save($term);
			}
		}
	}

	if (isset($form_state['values']['variable'])) {

		$varname = $types[$form_state['values']['type']]["variable"]["name"];
		variable_set($varname, $form_state['values']['variable']['value']);
	}

	drupal_set_message("Changes have been made.");
}

function theme_jmcouillard_manager_table($vars) {

	global $managed;

	$form = $vars['element'];
	$rows = array();
	$attributes = array('id' => array('jmcouillard_manager_form_table'));
	$name = $form["global"]["type"]["#value"];
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
	);

	// Extra colums header 
	if ($managed[$name]["extraColumns"]) {
		foreach ($managed[$name]["extraColumns"] as $extra) { 			
			$header[] = $form["#extra_data"]["extraColumns"][$extra]["label"];
		}
	}

	// Edit header colums header 
	$header[] = t('Edit');

	// Deletable table header
	if ($managed[$name]["deletable"]) {
		$header[] = t('Delete');
	}

	// Sortable table header
	if ($managed[$name]["sortable"]) {
		$header[] = t('Weight');
	}

	// dpm($managed);

	// For each item form element
	foreach (element_children($form['items']) as $key) {

		// Gete element to make it easier to access
		$element = $form['items'][$key];

		// Create row if element has an id
		$rows[$key] = array('data' => array(
				array('data' => $element['title']["#extra_data"]['img']),
				array('data' => $element['title']),
				// array('data' => $element['title']["#extra_data"]['edit_link']),
			), );


		// Extra colums
		if ($managed[$name]["extraColumns"]) {
			foreach ($managed[$name]["extraColumns"] as $extra) {
				$rows[$key]["data"][] = array('data' =>  $element[$extra]["#markup"]);
			}
		}

		// Edit field
		$rows[$key]["data"][] = array('data' => $element['title']["#extra_data"]['edit_link']);

		// Deletable field
		if ($managed[$name]["deletable"]) {
			$rows[$key]["data"][] = array('data' => $element['title']["#extra_data"]['delete_link']);
		}

		// Sortable field
		if ($managed[$name]["sortable"]) {
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

	// Render other form items
	$output .= drupal_render_children($form);

	// Add drag js
	if ($managed[$name]["sortable"])
		drupal_add_tabledrag('jmcouillard_manager_form_table', 'order', 'sibling', 'retouche-manager-weight');

	// Render and return
	return $output;
}

function jmcouillard_manager_form($form, &$form_state, $type = NULL, $name = NULL, $subcategory = NULL) {

	global $managed;

	// Set default name if not defined
	if(!isset($name)) {
		$keys = array_keys($managed);
		$name = array_shift($keys);
		$default = variable_get('jmcouillard_manager_default', $name );

		drupal_goto('admin/content/manager/' . $default);
	}

	// Define dynamic fields
	$weightField = variable_get('jmcouillard_manager_weight_field', "");
	$imageField = $managed[$name]["imageField"];
	$definedSubcagtegoryField = $managed[$name]["subcategoryField"];
	$thumbImageStyle = variable_get('jmcouillard_manager_thumb_style', "");
	$useForm = ($managed[$name]["form"] != "");

	// Define module path
	$modulePath = drupal_get_path('module', 'jmcouillard_manager');

	// Add custom css
	drupal_add_css($modulePath  . "/jmcouillard_manager_form_table.css");

	// Create form
	$form = array(
		'#theme' => 'jmcouillard_manager_table',
		'items' => array('#tree' => TRUE),
	);

	if($type == "node") {

		if($managed[$name]["sortable"]) {
			$results = db_query("SELECT * FROM {node} AS n LEFT JOIN {field_data_".$weightField."} AS w ON n.nid = w.entity_id WHERE n.type=:type ORDER BY " . $managed[$name]["order"], array(':type' => $name));
		} else {
			$results = db_query("SELECT * FROM {node} AS n WHERE n.type=:type ORDER BY n.created DESC", array(':type' => $name));
		}

	} else if ($type == "taxonomy_term") {
		$voc = taxonomy_vocabulary_machine_name_load($name);
		$results = db_query("SELECT * FROM {taxonomy_term_data} AS t WHERE t.vid=:vid ORDER BY t.weight", array(':vid' => $voc->vid));
	}

	$count = $results->rowCount();

	while ($row = $results -> fetch()) {

		if (!isset($row))
			break;

		// Load entity
		$entity = ($type == "node") ? node_load($row -> nid) : taxonomy_term_load($row -> tid);
		$entityId = ($type == "node") ? $entity -> nid : $entity -> tid;
		$entityTitle = ($type == "node") ? $entity -> title : $entity -> name;

		// Filter subcategory
		if(isset($subcategory)) {
			if (isset($entity -> {$definedSubcagtegoryField})) {

				$found = FALSE;
				foreach($entity->{$definedSubcagtegoryField}["und"] as $key => $item) {
					if($item["tid"] == $subcategory) $found = TRUE;
				}

				if(!$found) continue;

			} else {
				continue;
			}
		}

		// Create data
		$data = array(
			"nid"=>$entityId,
			"title"=>$entityTitle,
			"weight"=>0
		);

		// Define weight
		if($type == "node") {
			$data['weight'] = isset($entity -> {$weightField}['und'])  ? $entity -> {$weightField}['und'][0]['value'] : 0;
		} else if($type == "taxonomy_term") {
			$data['weight'] = $entity -> weight ;
		}

		// Link prefix
		$baseLinkPath = ($type == "node") ? "node/" : "taxonomy/term/";
		
		// Edit link
		if(!$useForm) {
			$data['edit_link'] = l(t('edit'), $baseLinkPath . $entityId . '/edit', array('query' => array('destination' => current_path())));
		} else {
			$data['edit_link'] = l(t('edit'), "admin/content/manager/edit/" . $name. "/" . $entityId . '/edit', array('query' => array('destination' => current_path())));			
		}
		
		// Delete link
		$data['delete_link'] = l(t('delete'),  $baseLinkPath . $entityId . '/delete', array('query' => array('destination' => current_path())));

		// Define image url (for thumbnail)
		if (isset($entity -> {$imageField}["und"])) {
			$data['img_url'] = image_style_url($thumbImageStyle, $entity -> {$imageField}["und"][0]["uri"]);
		} else {
			$data['img_url'] = $modulePath . "/images/jmcouillard_manager_default.png";
		}

		// Define image url
		$data['img'] = l(theme_image(array(
			"path" => $data['img_url'],
			"title" => $entityTitle,
			"attributes" => array(
				// "width" => 100,
				// "height" => 100
			)
		)), ("node/" . $entityId), array("html" => TRUE));

		$field_weight = array( 
			'#type' => 'weight',
			'#default_value' => $data['weight'],
			'#delta' => $count,
			'#attributes' => array('class' => array('retouche-manager-weight')),
			'#extra_data' => $data,
		);

		$field_title = array(
			'#type' => 'markup',
			'#markup' => $data['title'],
			'#extra_data' => $data,
		);

		$field_type = array(
			'#type' => 'hidden',
			'#value' => $type
		);

		$form['items'][$data['nid']] = array(
			"weight" => $field_weight,
			"title" => $field_title,
			"type" => $field_type
		);

		// Extra colums
		if (isset($managed[$name]["extraColumns"])) {
			foreach ($managed[$name]["extraColumns"] as $extra) {
				$form['items'][$data['nid']][$extra] = array(
					'#type' => 'markup',
					'#markup' => field_view_field($type, $entity, $extra, array("label"=>"hidden")),
					// '#extra_data' => field_view_field($type, $entity, $extra, array("label"=>"hidden")),
				);
			}
		}

		unset($row);
	};

	// Create extra columns data
	if (isset($managed[$name]["extraColumns"])) {
		foreach ($managed[$name]["extraColumns"] as $extra) {
			$infos = field_info_instance($type, $extra, $name);
			$form['#extra_data']['extraColumns'][$extra] = $infos;
		}
	}

	// Create textfield text area
	if (isset($managed[$name]["variable"])) {
		$form['global']['variable'] = array(
			'#name' => 'variable',
			'#type' => 'text_format',
			'#format' => "html",
			'#title' => $managed[$name]["variable"]["readable"],
			'#default_value' => variable_get($managed[$name]["variable"]["name"], ""),
			"#weight" => -100,
		);

		global $useSmallCKEditor;
		$useSmallCKEditor = TRUE;
	}

	// Create type hidden
	$form['global']['type'] = array(
		'#name' => 'type',
		'#type' => 'hidden',
		'#value' => $name
	);

	//Don't forget the submit button
	$form['actions'] = array('#type' => 'actions');
	$form['actions']['submit'] = array(
		'#name' => 'submit',
		'#type' => 'submit',
		'#value' => t('Save changes'),
	);

	$form['actions']['add'] = array(
		'#name' => 'add',
		'#type' => 'submit',
		'#value' => t('Add new'),
		'#submit' => array('_jmcouillard_manager_form_add'),
	);
	
	// Apply alter
	drupal_alter('jmcouillard_manager_actions_links', $form['actions'], $type);

	// Update title
	drupal_set_title("Manage " . str_replace("_", " ", $name)); 

	return $form;
}

function _jmcouillard_manager_form_add($form, &$form_state) {

	global $managed;

	if (isset($form_state['values'])) {
		$type = $form_state["build_info"]["args"][0];
		$name = $form_state["build_info"]["args"][1];
		$useForm = ($managed[$name]["form"] != "");
		
		if($useForm) {
			drupal_goto("admin/content/manager/add/" . $name);
		}else {
			if($type=="node") drupal_goto("node/add/" . str_replace("_", "-", $name));
			else if($type=="taxonomy_term") drupal_goto("admin/structure/taxonomy/" . $name . "/add");
		}
	};
}


?>