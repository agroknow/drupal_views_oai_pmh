<?php

/**
 * @file
 * Theme related functions for processing our output style plugins.
 *
 * Views bug: http://drupal.org/node/593336
 */



function theme_views_oai_pmh_row_dc_fields(&$vars) {
  $view = $vars['view'];

  // Loop through the fields for this view.
  $previous_inline = FALSE;
  $vars['fields'] = array(); // ensure it's at least an empty array.
  $row_index = $view->row_index;
  foreach ($view->field as $id => $field) {
    // render this even if set to exclude so it can be used elsewhere.
    $field_output = $view->style_plugin->get_field($row_index, $id);
    $empty = $field_output !== 0 && empty($field_output);
    if (empty($field->options['exclude']) && (!$empty || (empty($field->options['hide_empty']) && empty($vars['options']['hide_empty'])))) {
      $key = check_plain(_views_oai_pmh_xml_tag_clean($view->field[$id]->label()));
      if (isset($field->multiple) && !empty($field->multiple)) {
        $separator = $field->options['separator'];
        foreach (explode($separator, $field_output) as $value) {
          $item['key'] = $key;
          $item['value'] = _views_oai_pmh_xmlstr($value);
          $items[] = $item;
        }
      }
      else {
        $item['key'] = $key;
        $item['value'] = _views_oai_pmh_xmlstr($field_output);
        $items[] = $item;
      }
    }
  }
 return format_xml_elements($items);

}

//added for okhub AP
function theme_views_oai_pmh_row_okhub_fields(&$vars) {
  $view = $vars['view'];

  // Loop through the fields for this view.
  $previous_inline = FALSE;
  $vars['fields'] = array(); // ensure it's at least an empty array.
  $row_index = $view->row_index;
  $iLanTitle=0;
  $iLanabstract=0;
  //$iLanAltTitle=0;
  foreach ($view->field as $id => $field) {
  $iLanTitle=0;
    // render this even if set to exclude so it can be used elsewhere.
    $field_output = $view->style_plugin->get_field($row_index, $id); 

    $empty = $field_output !== 0 && empty($field_output);
    if (empty($field->options['exclude']) && (!$empty || (empty($field->options['hide_empty']) && empty($vars['options']['hide_empty'])))) {
      $key = check_plain(_views_oai_pmh_xml_tag_clean($view->field[$id]->label()));
      

      if (isset($field->multiple) && !empty($field->multiple)) {
        $separator = $field->options['separator'];
      }else{
        //in order to avoid separate one value if the value has ","
        $separator = '---------------------!!----------';
      }
        
        foreach (explode($separator, $field_output) as $value) {
   
//multiple values....

          switch ($key) {
            case "dcterms:title":
            case "dcterms:alternative":
            case "dcterms:rights":
              $item['key'] = $key;
              $item['value'] = _views_oai_pmh_xmlstr($value);

              $lang = find_language_attr_for_field($view,$row_index,$key.'_language',$iLanTitle);
              $iLanTitle++;
              $item['attributes'] = array('xml:lang'=>$lang);//put the language of abstract but it is not a view field
              $items[] = $item;

              break;
            case "dcterms:creator":
            case "dcterms:contributor":
              $sub2['key'] = "foaf:name";
              $sub2['value'] = _views_oai_pmh_xmlstr($value);
              $sub2['attributes'] = '';

              $sub['key'] = "foaf:Person";
              //$sub['value'] = _views_oai_pmh_xmlstr($value);
              $sub['value'] =  array('0' => $sub2);
              $sub['attributes'] = '';

              $item['key'] = $key;
              $item['value'] = array('0' => $sub);
              $item['attributes'] = '';

              $items[] = $item;
            break;
            case "dcterms:publisher":
              $sub2['key'] = "foaf:name";
              $sub2['value'] = _views_oai_pmh_xmlstr($value);
              $sub2['attributes'] = '';

              $sub['key'] = "foaf:Organization";
              //$sub['value'] = _views_oai_pmh_xmlstr($value);
              $sub['value'] =  array('0' => $sub2);
              $sub['attributes'] = '';

              $item['key'] = $key;
              $item['value'] = array('0' => $sub);
              $item['attributes'] = '';

              $items[] = $item;
            break;
            case "bibo:presentedAt":
              $sub2['key'] = "foaf:name";
              $sub2['value'] = _views_oai_pmh_xmlstr($value);
              $sub['attributes'] = '';

              $sub['key'] = "bibo:conference";
              //$sub['value'] = _views_oai_pmh_xmlstr($value);
              $sub['value'] =  array('0' => $sub2);
              $sub['attributes'] = '';

              $item['key'] = $key;
              $item['value'] = array('0' => $sub);
              $item['attributes'] = '';

              $items[] = $item;
            break;
            case "edm:isShownBy":
            case "edm:isShownAt":
              $sub['key'] = $key;
              $sub['value'] = _views_oai_pmh_xmlstr($value);
              $sub['attributes'] = '';


              $item['key'] = 'edm:hasView';
              $item['value'] = array('0' => $sub);
              $item['attributes'] = array('xsi:type'=>'dcterms:URI');

              $items[] = $item;
            break;
            case "dcterms:medium":
            case "dcterms:extent":
              $sub['key'] = $key;
              $sub['value'] = _views_oai_pmh_xmlstr($value);
              $sub['attributes'] = '';


              $item['key'] = 'dcterms:format';
              $item['value'] = array('0' => $sub);
              $item['attributes'] = '';

              $items[] = $item;
            break;
            case "dcterms:date":

              $item['key'] = $key;
              $item['value'] = _views_oai_pmh_xmlstr($value);
              $item['attributes'] = array('xsi:type'=>'dcterms:W3CDTF');

              $items[] = $item;
            break;
            case "foaf:based_near":
              $sub2['key'] = "schema:Name";
              $sub2['value'] = _views_oai_pmh_xmlstr($value);
              $sub2['attributes'] = '';

              $sub['key'] = $key;
              //$sub['value'] = _views_oai_pmh_xmlstr($value);
              $sub['value'] =  '';
              $sub['attributes'] = '';

              $sub1['key'] = 'schema:Country';
              //$sub['value'] = _views_oai_pmh_xmlstr($value);
              $sub1['value'] =  '';
              $sub1['attributes'] = '';

              $item['key'] = 'foaf:Organization';
              $item['value'] = array('0' => $sub,'1' => $sub1,'2' => $sub2);
              $item['attributes'] = '';

              $items[] = $item;
            break;
            case "dcterms:language":

              $item['key'] = $key;
              $item['value'] = _views_oai_pmh_xmlstr($value);
              $item['attributes'] = array('xsi:type'=>'dcterms:ISO639-1');

              $items[] = $item;
            break;
            case "dcterms:abstract":
              $sub['key'] = $key;
              $sub['value'] = _views_oai_pmh_xmlstr($value);
              $lang = find_language_attr_for_field($view,$row_index,$key.'_language',$iLanTitle);
              $iLanTitle++;
              $sub['attributes'] = array('xml:lang'=>$lang);


              $item['key'] = 'dcterms:description';
              $item['value'] = array('0' => $sub);
              $item['attributes'] = '';

              $items[] = $item;
            break;
            case "dcterms:spatial_country":
            case "dcterms:spatial_region":
            case "dcterms:spatial_continent":
              $sub['key'] = 'dcterms:spatial';
              $sub['value'] = _views_oai_pmh_xmlstr($value);
              if($key=='spatial_country'){
               $sub['attributes'] = array('scheme'=>'dcterms:ISO3166','xsi:schemaLocation'=>'country'); 
              }elseif($key=='spatial_region'){
                $sub['attributes'] = array('scheme'=>'dcterms:ISO3166','xsi:type'=>'region');

              }elseif($key=='spatial_continent'){
                $sub['attributes'] = array('scheme'=>'dcterms:ISO3166','xsi:type'=>'continent'); 
              }
              

              $item['key'] = 'dcterms:coverage';
              $item['value'] = array('0' => $sub);
              $item['attributes'] = '';

              $items[] = $item;

            break;
            case "dcterms:accessRights":
            case "dcterms:license":
              $sub['key'] = $key;
              $sub['value'] = _views_oai_pmh_xmlstr($value);
              $sub['attributes'] = '';


              $item['key'] = 'dcterms:rights';
              $item['value'] = array('0' => $sub);
              $item['attributes'] = '';

              $items[] = $item;
            break;
            case "dcterms:subject_keywords":
              $item['key'] = 'dcterms:subject';
              $item['value'] = _views_oai_pmh_xmlstr($value);

              $lang = find_language_attr_for_field($view,$row_index,$key.'_language',$iLanTitle);
              $iLanTitle++;
              $item['attributes'] = array('xml:lang'=>$lang);//put the language of abstract but it is not a view field
              $items[] = $item;

              break;
              case "dcterms:subject_subject":
              $item['key'] = 'dcterms:subject';
              $item['value'] = _views_oai_pmh_xmlstr($value);

              $item['attributes'] = array('scheme'=>'ags:AGROVOC');//put the language of abstract but it is not a view field
              $items[] = $item;

              break;
              case "dcterms:identifier_metadata":
              $item['key'] = 'dcterms:identifier';
              $item['value'] = _views_oai_pmh_xmlstr($value);

              $item['attributes'] = array('xsi:schemaLocation'=>'metaId');//put the language of abstract but it is not a view field
              $items[] = $item;

              break;
              case "foaf:creator_organization_metadata":
              case "foaf:creator_person_metadata":

              $sub2['key'] = "foaf:name";
              $sub2['value'] = _views_oai_pmh_xmlstr($value);
              $sub2['attributes'] = '';

              if($key=='foaf:creator_organization_metadata'){
                $sub['key'] = "foaf:Organization";
              }else{
                $sub['key'] = "foaf:Person";
              }              
              //$sub['value'] = _views_oai_pmh_xmlstr($value);
              $sub['value'] =  array('0' => $sub2);
              $sub['attributes'] = '';

              $item['key'] = 'dcterms:creator';
              $item['value'] = array('0' => $sub);
              $item['attributes'] =  array('xsi:schemaLocation'=>'metaCreator');

              $items[] = $item;
            break;
            case "edm:dataProvider_metadata":

              $sub2['key'] = "foaf:name";
              $sub2['value'] = _views_oai_pmh_xmlstr($value);
              $sub2['attributes'] = '';


              $sub['key'] = "foaf:Organization";       
              //$sub['value'] = _views_oai_pmh_xmlstr($value);
              $sub['value'] =  array('0' => $sub2);
              $sub['attributes'] = '';

              $sub3['key'] = 'edm:dataProvider';
              $sub3['value'] = array('0' => $sub);
              $sub3['attributes'] =  '';

              $item['key'] = 'dcterms:provenance';
              $item['value'] = array('0' => $sub3);
              $item['attributes'] =  '';

              $items[] = $item;
            break;
            case "dcterms:dateCreated_metadata":

              $sub['key'] = 'dateCreated';
              $sub['value'] = _views_oai_pmh_xmlstr($value);
              $sub['attributes'] = '';


              $item['key'] = 'dcterms:date';
              $item['value'] = array('0' => $sub);
              $item['attributes'] =   array('xsi:schemaLocation'=>'metaCreatedDate','xsi:type'=>'dcterms:W3CDTF');


              $items[] = $item;
              break;
              case "dcterms:date_metadataingestion":
              case "dcterms:date_metadataupdate":
                $item['key'] = 'dcterms:date';
                $item['value'] = _views_oai_pmh_xmlstr($value);
                if($key=='dcterms:date_metadataingestion'){
                  $item['attributes'] =   array('xsi:schemaLocation'=>'metaIngestionDate','xsi:type'=>'dcterms:W3CDTF');
                }else{
                  $item['attributes'] =   array('xsi:schemaLocation'=>'metaUpdatedDate','xsi:type'=>'dcterms:W3CDTF');
                }
                
                $items[] = $item;
              break;
              case "dcterms:title_citation":
                $item['key'] = 'dcterms:title';
                $item['value'] = _views_oai_pmh_xmlstr($value);
                $lang = find_language_attr_for_field($view,$row_index,$key.'_language',$iLanTitle);
                $iLanTitle++;
                $item['attributes'] =   array('xsi:schemaLocation'=>'CitationTitle','xml:lang'=>$lang);

                
                $items[] = $item;
              break;
              case "bibo:uri_citation":
                $item['key'] = 'bibo:ISSN';
                $item['value'] = _views_oai_pmh_xmlstr($value);
                $item['attributes'] = '';
                
                $items[] = $item;
              break;
              case "bibo:volume_citation":
                $item['key'] = 'bibo:issue';
                $item['value'] = _views_oai_pmh_xmlstr($value);
                $item['attributes'] = '';
                
                $items[] = $item;
              break;
              case "dcterms:date_citation":
                $item['key'] = 'dcterms:date';
                $item['value'] = _views_oai_pmh_xmlstr($value);
                $item['attributes'] =   array('xsi:schemaLocation'=>'CitationChronology','xsi:type'=>'dcterms:W3CDTF');
                
                $items[] = $item;
              break;
               
            

              case "dcterms:title_language":
              case "dcterms:abstract_language":
              case "dcterms:alternative_language":
              case "dcterms:rights_language":
              case "dcterms:subject_keywords_language":
              case "dcterms:title_citation_language":
              break;
              case "dcterms:identifier":
              case "bibo:edition":
              case "dcterms:type":
              case "dcterms:relation":
              case "dcterms:isPartOf":
              case "dcterms:hasPart":
              case "dcterms:hasVersion":
              case "dcterms:isVersionOf":
              case "dcterms:relation":
              case "dcterms:hasTranslation":
              case "dcterms:isTranslationOf":
              case "dcterms:references":
              case "dcterms:isReferencedBy":
              case "dcterms:replaces":
              case "dcterms:isReplacedBy":
              case "dcterms:isRequiredBy":
              case "dcterms:requires":
              default:
              $item['key'] = $key;
              $item['value'] = _views_oai_pmh_xmlstr($value);
              $item['attributes'] = '';
              $items[] = $item;

        }      
          
        }
      // }
      
      // else {
      //   // one value each field

      //       $item['key'] = $key;
      //       $item['value'] = $field_output;
      //       $item['attributes'] = '';
      //       $items[] = $item;

      // }
    }
  }
 return format_xml_elements($items);

}

//check for fied title_language
//loop through fields to find specific value
function find_language_attr_for_field($view,$row_index,$lanfield,$flag){
    foreach($view->field as $tesk => $tes){
              $test = check_plain(_views_oai_pmh_xml_tag_clean($view->field[$tesk]->label()));
              
                if($test==$lanfield){
                  $lang_value = $view->style_plugin->get_field($row_index, $tesk);
                  if (isset($tes->multiple) && !empty($tes->multiple)) {
                    $separator_language = $tes->options['separator'];
        
                    $langs = explode($separator_language, $lang_value);
                    $lang = $langs[$flag];
                  
                  }else{
                    $lang = $lang_value;
                  }
              }
            }
            return $lang;

}

function template_preprocess_views_oai_pmh_response(&$vars) {
  global $base_url;

  $vars['oai_response_date'] = format_xml_elements(array('responseDate' => $vars['oai_args']['response_date']));
  $request = array('key' => 'request',
        					 'value' => $base_url);
  $request += !empty($vars['oai_args']['verb']) ?
                array('attributes' => array('verb' => $vars['oai_args']['verb'])): array();
  $vars['oai_request'] = format_xml_elements(array($request));
  $vars['oai_verb'] = !empty($vars['oai_args']['verb']) ? $vars['oai_args']['verb']: '';
  $vars['oai_base_url'] = $base_url;
  $vars['oai_errors'] = format_xml_elements($vars['oai_args']['errors']);

  $vars['oai_content'] = $vars['oai_content'];
  if (!empty($vars['oai_content'])) {
    $vars['oai_resumption_token'] = $vars['resumption_token'];
  }
}

function theme_views_oai_pmh_identify_body($vars) {
  global $base_url;
  $view = $vars['view'];
  $path = $view->display_handler->get_path();
  $datestamp = db_query("SELECT min(changed) FROM {node} ")->fetchField();

  $ident = array(
  	'repositoryName' => variable_get('site_name', 'Views OAI'),
    'baseURL' => $base_url . '/' . $path,
    'protocolVersion' => '2.0',
    'adminEmail' => variable_get('site_mail', ini_get('sendmail_from')),
    'earliestDatestamp' => gmstrftime('%Y-%m-%dT%H:%M:%SZ', $datestamp),
    'deletedRecord' => 'no',
    'granularity' => 'YYYY-MM-DDThh:mm:ssZ',
    'compression' => 'gzip',
  );

  return format_xml_elements($ident);
}

function theme_views_oai_pmh_metadataformats_body($vars) {
  $formats = '';
  $formats .= format_xml_elements(array ( '0'=> array('key'=> 'metadataFormat' ,'value' => array(
    'metadataPrefix' => 'oai_dc',
  'schema' => 'http://www.openarchives.org/OAI/2.0/oai_dc.xsd',
    'metadataNamespace' => 'http://www.openarchives.org/OAI/2.0/oai_dc/',
  )),
  //    //anto added metadataform agris ap but in future it need to be dynamic
  //    '1'=> array('key'=>'metadataFormat' ,'value' => array(
  //   'metadataPrefix' => 'agrisap',
  // 'schema' => 'http://www.openarchives.org/OAI/2.0/oai_ags/',
  //   'metadataNamespace' => 'http://purl.org/agmes/1.1/',
  // )),
     //added for okhub AP
     '2'=> array('key'=>'metadataFormat' ,'value' => array(
    'metadataPrefix' => 'okhub',
  'schema' => 'http://www.openarchives.org/OAI/2.0/oai_ags/',
    'metadataNamespace' => 'http://purl.org/agmes/1.1/',
  ))

     ));
  return $formats;
}

function theme_views_oai_pmh_record_header($vars) {
  global $base_url;
  static $parts = array();
  if (empty($parts)) {
    $parts = parse_url($base_url);
  }
  $nid = $vars['nid'];
  $datestamp = $vars['node_changed'];

  $info['header']['identifier'] = 'oai:' . $parts['host'] . ':' . $nid;
  $info['header']['datestamp'] = gmstrftime('%Y-%m-%dT%H:%M:%SZ', $datestamp);
//  $info['header']['setSpec'] = '';

  return format_xml_elements($info);
}

function theme_views_oai_pmh_dc_record($vars) {
  $nid = $vars['nid'];
  $datestamp = $vars['node_changed'];
  $header = theme('views_oai_pmh_record_header', array('nid' => $nid, 'node_changed' => $datestamp));

  return '<record>' .
    $header .
    '<metadata>
      <oai_dc:dc  xmlns:oai_dc="http://www.openarchives.org/OAI/2.0/oai_dc/"
                      xmlns:dc="http://purl.org/dc/elements/1.1/"
                      xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                      xsi:schemaLocation="http://www.openarchives.org/OAI/2.0/oai_dc/  http://www.openarchives.org/OAI/2.0/oai_dc.xsd">' .
      $vars['fields'] .
      '</oai_dc:dc>
    </metadata>
   </record>';
}

// added for okhub AP
function theme_views_oai_pmh_okhub_record($vars) { 
  $nid = $vars['nid'];
  $datestamp = $vars['node_changed'];
  $header = theme('views_oai_pmh_record_header', array('nid' => $nid, 'node_changed' => $datestamp));

  return '<record>' .
    $header .
    '<metadata>
      <oai_dc:dc  xmlns:oai_dc="http://www.openarchives.org/OAI/2.0/oai_dc/"
                      xmlns:foaf="http://xmlns.com/foaf/0.1" 
                      xmlns:edm="http://pro.europeana.eu/share-your-data/data-guidelines/edm-documentation"
                      xmlns:bibo="http://purl.org/ontology/bibo/" 
                      xmlns:dcterms="http://purl.org/dc/terms/" 
                      xmlns:skos="http://www.w3.org/2008/05/skos" 
                      xmlns:geop="http://www.fao.org/countryprofiles/geoinfo/geopolitical/resource/geopolitical.owl" 
                      xmlns:schema="http://schema.org/" 
                      xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
                      xmlns:ags="http://purl.org/agmes/1.1/" xsi:schemaLocation="http://www.w3schools.com meta.xsd">' .
      $vars['fields'] .
      '</oai_dc:dc>
    </metadata>
   </record>';
}


/**
 * Returns a valid XML tag formed from the given input.
 *
 * @param $tag The string that should be made into a valid XML tag.
 * @return The valid XML tag or an empty string if the string contained no valid
 * XML tag characters.
 */
function _views_oai_pmh_xml_tag_clean($tag) {

  // This regex matches characters that are not valid in XML tags, and the
  // unicode ones that are. We don't bother with unicode, because it would so
  // the preg_replace down a lot.
  static $invalid_tag_chars_regex = '#[^\:A-Za-z_\-.0-9]+#';

  // These characters are not valid at the start of an XML tag:
  static $invalid_start_chars = '-.0123456789';

  // Convert invalid chars to '-':
  $tag = preg_replace($invalid_tag_chars_regex, '-', $tag);

  // Need to trim invalid characters from the start of the string:
  $tag = ltrim($tag, $invalid_start_chars);

  return $tag;
}

function theme_oai_field_mapper_form($vars) { 
  $form = $vars['form'];
  foreach (element_children($form) as $key ) {
      $rows[] = array(
        drupal_render($form[$key]['drupal_label']),
        drupal_render($form[$key])
      );
  }
  $header = array(
    t('Drupal field label'),
    t('Dublin Core element name')
  );
  $output = theme('table', array('header' => $header, 'rows' => $rows));

  $output .= drupal_render_children($form);
  return $output;

}
function theme_oai_field_okhub_mapper_form($vars) { 
  $form = $vars['form'];
  foreach (element_children($form) as $key ) {
      $rows[] = array(
        drupal_render($form[$key]['drupal_label']),
        drupal_render($form[$key])
      );
  }
  $header = array(
    t('Drupal field label'),
    t('OKhub AP element name')
  );
  $output = theme('table', array('header' => $header, 'rows' => $rows));

  $output .= drupal_render_children($form);
  return $output;

}

function _views_oai_pmh_xmlstr($string, $charset = 'iso8859-1', $xmlescaped = 'false')
{
  $xmlstr = stripslashes(trim($string));
  // just remove invalid characters
  $pattern ="/[\x-\x8\xb-\xc\xe-\x1f]/";
  $xmlstr = preg_replace($pattern, '', $xmlstr);

  // escape only if string is not escaped
  if (!$xmlescaped) {
    $xmlstr = htmlspecialchars($xmlstr, ENT_QUOTES);
  }

  return $xmlstr;
}
