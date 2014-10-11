<?php

/**
 * Implementation of hook_field_formatter_info(). It routes pages to functions.
 */
function cake_public_field_formatter_info() {

	$items = array();

	$items['cake_public_pinkbutton'] = array(
		'label' => 'Pink button',
		'description' => 'Give me an url, I will give you an pink button.',
		'field types' => array('text'),
		'settings' => array(
			'cake_public_pinkbutton_label' => 'Visit',
			'cake_public_pinkbutton_gaevent' => ''
		),
	);
	$items['cake_public_title'] = array(
		'label' => 'Title',
		'description' => 'Render a text withion a <h> wrapper.',
		'field types' => array('text'),
		'settings' => array('cake_public_title_wrapper' => 'h1'),
	);
	$items['cake_public_imagelink'] = array(
		'label' => 'Image link button',
		'description' => 'Give me an url, I will give you an image link button.',
		'field types' => array('text'),
		'settings' => array('cake_public_imagelink_type' => 'fb'),
	);

	return $items;
}

/**
 * Implementation of hook_field_formatter_view()
 */
function cake_public_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {

	$element = array();
	$settings = $display['settings'];

	$classes = array(
		"fb" => "fa-facebook-square",
		"tw" => "fa-twitter",
		"li" => "fa-linkedin",
		"web" => "fa-external-link",
	);

	switch ($display['type']) {
		case 'cake_public_pinkbutton' :
			$label = $settings['cake_public_pinkbutton_label'];
			$gaevent = isset($settings['cake_public_pinkbutton_gaevent']) ? $settings['cake_public_pinkbutton_gaevent'] : "";
			foreach ($items as $delta => $item) {
				$href = url($item['value']);
				if(!empty($href)) {
					$onclick = !empty($gaevent) ? ' onClick="ga_event([\'' . $gaevent . '\', \'Click\', \'' . $entity->title . '\']);"' : "";
					$element[$delta] = array('#markup' => '<div class="button-pink"><a href="' . $href . '" target="_blank"' . $onclick . '>' . t($label) . '</a></div>');
				}
			}
			break;
		case 'cake_public_imagelink' :
			$type = $settings['cake_public_imagelink_type'];
			$fa = $classes[$type];
			foreach ($items as $delta => $item) {
				$href = url($item['value']);
				if(empty($href))
					break;
				$element[$delta] = array('#markup' => '<a class="imagelink" href="' . $href . '" target="_blank"><i class="fa ' . $fa . '"></i></a>');
			}
			break;
		case 'cake_public_title' :
			$wrapper = $settings['cake_public_title_wrapper'];
			foreach ($items as $delta => $item) {
				$element[$delta] = array('#markup' => '<' . $wrapper . '>' . $item['value'] . '</' . $wrapper . '>');
			}
			break;
	}

	return $element;
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function cake_public_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
	$display = $instance['display'][$view_mode];
	$settings = $display['settings'];
	$form = array();

	if($display['type'] == 'cake_public_pinkbutton') {

		$form['cake_public_pinkbutton_label'] = array(
			'#title' => t("Label"),
			'#type' => 'textfield',
			'#default_value' => $settings['cake_public_pinkbutton_label'],
			'#required' => TRUE,
		);

		$form['cake_public_pinkbutton_gaevent'] = array(
			'#title' => t("GA event"),
			'#type' => 'textfield',
			'#default_value' => $settings['cake_public_pinkbutton_gaevent'],
			'#required' => FALSE,
		);

	} else if($display['type'] == 'cake_public_imagelink') {

		$form['cake_public_imagelink_type'] = array(
			'#title' => t("Label"),
			'#type' => 'select',
			'#options' => array(
				'fb' => t('Facebook'),
				'tw' => t('Twitter'),
				'li' => t('Linkedin'),
				'web' => t('Website'),
			),
			'#default_value' => $settings['cake_public_imagelink_type'],
			'#required' => TRUE,
		);

	} else if($display['type'] == 'cake_public_title') {

		$form['cake_public_title_wrapper'] = array(
			'#title' => t("Wrapper"),
			'#type' => 'select',
			'#options' => array(
				'h1' => 'h1',
				'h2' => 'h2',
				'h3' => 'h3',
			),
			'#default_value' => $settings['cake_public_title_wrapper'],
			'#required' => TRUE,
		);

	}

	$context = array(
		'field' => $field,
		'instance' => $instance,
		'view_mode' => $view_mode
	);

	drupal_alter('cake_public_field_formatter_settings_form', $form, $form_state, $context);

	return $form;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function cake_public_field_formatter_settings_summary($field, $instance, $view_mode) {
	$display = $instance['display'][$view_mode];
	$settings = $display['settings'];

	$summary = array();

	if($display['type'] == 'cake_public_pinkbutton') {
		$summary[] = t("Label is ") . $settings['cake_public_pinkbutton_label'];
		if(isset($settings['cake_public_pinkbutton_gaevent'])) {
			$summary[] = t("GA Event is ") . $settings['cake_public_pinkbutton_gaevent'];
		}
		$summary = theme('item_list', array(
			'type' => 'ul',
			'items' => $summary
		));
	} else if($display['type'] == 'cake_public_imagelink') {
		$summary[] = t("Type: ") . $settings['cake_public_imagelink_type'];
		$summary = theme('item_list', array(
			'type' => 'ul',
			'items' => $summary
		));
	} else if($display['type'] == 'cake_public_title') {
		$summary[] = t("Wrapper: ") . $settings['cake_public_title_wrapper'];
		$summary = theme('item_list', array(
			'type' => 'ul',
			'items' => $summary
		));
	}

	return $summary;
}
