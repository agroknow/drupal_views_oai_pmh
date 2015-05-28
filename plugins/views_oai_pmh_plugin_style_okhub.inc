<?php
/**
 * @file
 */

class views_oai_pmh_plugin_style_okhub extends views_oai_pmh_plugin_style {
  function render_records() {
    if (!empty($this->display->handler->oai_args['errors'])) return;
   // $records = array();
    if ($this->uses_row_plugin() && empty($this->row_plugin)) {
      vpr('views_plugin_style_default: Missing row plugin');
      return;
    }
    $records = '';
    if ($this->uses_row_plugin()) {
      $rows = array();
      foreach ($this->view->result as $row_index => $row) {
        $this->view->row_index = $row_index;
        $okhub_fields = $this->row_plugin->render($row);
        $records .= theme('views_oai_pmh_okhub_record', array('nid' => $row->nid, 'node_changed' => $row->node_changed, 'fields' => $okhub_fields));
      }
    }
  return $records;
  }

}
