<?php
class views_oai_pmh_plugin_row_okhub extends views_plugin_row {

  function render($row) {

    return theme($this->theme_functions(),
    array(
          'view' => $this->view,
          'options' => $this->options,
          'row' => $row
    ));
  }

  function validate() {
    $errors = parent::validate();
    $okhub_errors = array();
    $link = $this->display->handler->option_link('Row options', 'row_options');

    $field_handlers = $this->display->handler->get_handlers('field');
    foreach ($field_handlers as $id => $field) {
      $ui_label = $field->ui_name();
      $label = $field->label();
      if (empty($label) || (substr($label, 0, 3) != 'dct:' ) && (substr($label, 0, 8) != 'dcterms:') && (substr($label, 0, 5) != 'bibo:') && (substr($label, 0, 4) != 'edm:') && (substr($label, 0, 5) != 'foaf:')) {
        $okhub_errors[] = t('The field "@title" does not have an OK hub AP label associated with it. Go to the !link page to fix it.', array('@title' => $ui_label, '!link' => $link));
      }
    }
    return array_merge($errors, $okhub_errors);
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $field_handlers = $this->display->handler->get_handlers('field');

    foreach ($field_handlers as $id => $handler) {
      $ui_labels[$id] = $handler->ui_name();
      if ($label = $handler->label()) {
        $labels[$id] = $label;
      }
    }

    $okhub_elements = array(
      'none'					 => t('None'),
      'dcterms:title'			  => 'dcterms:title',
      'dcterms:title_language'      => 'dcterms:title_language',
      'dcterms:alternative' => 'dcterms:alternative',
      'dcterms:alternative_language' => 'dcterms:alternative_language',
      'dcterms:creator'     => 'dcterms:creator',
      'dcterms:publisher'   => 'dcterms:publisher',
      'dcterms:contributor' => 'dcterms:contributor',
      'bibo:presentedAt'    => 'bibo:presentedAt',
      'edm:isShownBy'       => 'edm:isShownBy',
      'edm:isShownAt'       => 'edm:isShownAt',
      'dcterms:identifier'  => 'dcterms:identifier',
      'dcterms:medium'      => 'dcterms:medium',
      'dcterms:extent'      => 'dcterms:extent',
      'dcterms:date'  => 'dcterms:date',
      'foaf:based_near'     => 'foaf:based_near',
      'dcterms:language'    => 'dcterms:language',
      'bibo:edition'        => 'bibo:edition',
      'dcterms:abstract'    => 'dcterms:abstract',
      'dcterms:abstract_language'      => 'dcterms:abstract_language',
      'dcterms:type'        => 'dcterms:type',
      'dcterms:spatial_country'     => 'dcterms:spatial_country',
      'dcterms:spatial_region'  => 'dcterms:spatial_region',
      'dcterms:spatial_continent'   => 'dcterms:spatial_continent',
      'dcterms:accessRights'=> 'dcterms:accessRights',
      'dcterms:license'     => 'dcterms:license',
      'dcterms:rights'      => 'dcterms:rights',
      'dcterms:rights_language'    => 'dcterms:rights_language',
      'dcterms:subject_subject'    => 'dcterms:subject_subject',
      'dcterms:subject_keywords'   => 'dcterms:subject_keywords',
      'dcterms:subject_keywords_language'   => 'dcterms:subject_keywords_language',
      'dcterms:relation'    => 'dcterms:relation',
      'dcterms:isPartOf'    => 'dcterms:isPartOf',
      'dcterms:hasPart'     => 'dcterms:hasPart',
      'dcterms:hasVersion'  => 'dcterms:hasVersion',
      'dcterms:isVersionOf' => 'dcterms:isVersionOf',
      'dcterms:relation'    => 'dcterms:relation',
      'dcterms:hasTranslation'  => 'dcterms:hasTranslation',
      'dcterms:isTranslationOf' => 'dcterms:isTranslationOf',
      'dcterms:references'      => 'dcterms:references',
      'dcterms:isReferencedBy'  => 'dcterms:isReferencedBy',
      'dcterms:replaces'        => 'dcterms:replaces',
      'dcterms:isReplacedBy'    => 'dcterms:isReplacedBy',
      'dcterms:isRequiredBy'    => 'dcterms:isRequiredBy',
      'dcterms:requires'        => 'dcterms:requires',
      'dcterms:identifier_metadata'  => 'dcterms:identifier_metadata',
      'foaf:creator_organization_metadata'   => 'foaf:creator_organization_metadata',
      'foaf:creator_person_metadata'         => 'foaf:creator_person_metadata',
      'edm:dataProvider_metadata'    => 'edm:dataProvider_metadata',
      'dcterms:dateCreated_metadata' => 'dcterms:dateCreated_metadata',
      'dcterms:date_metadataingestion'     => 'dcterms:date_metadataingestion',
      'dcterms:date_metadataupdate'        => 'dcterms:date_metadataupdate',
      'dcterms:title_citation'       => 'dcterms:title_citation',
      'dcterms:title_citation_language'       => 'dcterms:title_citation_language',
      'bibo:uri_citation'            => 'bibo:uri_citation',
      'bibo:volume_citation'         => 'bibo:volume_citation',
      'dcterms:date_citation'        => 'dcterms:date_citation',
    );

    $form['oai_labels'] = array(
      '#type' => 'fieldset',
      '#title' => t('Drupal field to OKhub AP mapping'),
      '#theme' => 'oai_field_okhub_mapper_form',
    );

    foreach ($ui_labels as $id => $label) {
      $form['oai_labels'][$id] = array(
        '#type' => 'select',
        '#options' => $okhub_elements,
        '#default_value' => isset($labels[$id]) && in_array($labels[$id], $okhub_elements) ? $labels[$id] : 'none',
      );
      $form['oai_labels'][$id]['drupal_label'] = array(
        '#markup' => $ui_labels[$id],
      );
    }
  }
  function options_submit(&$form, &$form_state) {
    // It is very important to call the parent function here:
    parent::options_submit($form, $form_state);
    $section = $form_state['section'];
    switch ($section) {
      case 'row_options':
        $field_handlers = $this->display->handler->get_handlers('field');
        $okhub_labels = $form_state['values'][$section]['oai_labels'];
        foreach ($field_handlers as $id => $field) {
          $options = $field->options;
          if ($okhub_labels[$id] != 'none') {
            $options['custom_label'] = TRUE;
            $options['label'] = $okhub_labels[$id];
            $options['hide_empty'] = 1;
          }
          else {
            unset($options['custom_label']);
            $options['label'] = '';
          }
          $form_state['view']->set_item($form_state['display_id'], 'field', $id, $options);
        }
        break;
    }
  }
}
