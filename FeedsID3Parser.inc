<?php

/**
 * @files
 * Provides the FeedsXPathParserXML class.
 */
class FeedsID3Parser extends FeedsParser {

  /**
   * Implements FeedsParser::parse().
   */
  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result) {

    $id3_file = $fetcher_result->getFilePath();
    $id3_file_realpath = drupal_realpath($id3_file);

    $getID3 = getid3_instance();
    $getID3->encoding = 'UTF-8';
    $info = $getID3->analyze($id3_file_realpath);

    $format_name = (isset($info['fileformat']) ? $info['fileformat'] : '').'/'.(isset($info['audio']['dataformat']) ? $info['audio']['dataformat'] : '').(isset($info['video']['dataformat']) ? '/'.$info['video']['dataformat'] : '');
    $encoder_version = (isset($info['audio']['encoder']) ? $info['audio']['encoder'] : '');
    $playing_time = (isset($info['playtime_seconds']) ? $info['playtime_seconds'] : '');
    $tags = (isset($info['tags']) ? $info['tags'] : '');
    $comments = $info['id3v2']['comments'];
    $picture = $info['id3v2']['APIC'][0]['data'];
    $picture_path = file_unmanaged_save_data($picture, file_directory_temp());

    if (isset($this->info['error'])) {
      throw new Exception(t('FeedsID3Parser: ' . $info['error'] ));
    }

    $item = array(
      'file' => $id3_file_realpath,
      'format_name' => $format_name,
      'encoder_version' => $encoder_version,
      'title' => isset( $info['tags']['id3v2']['title'][0] ) ? $info['tags']['id3v2']['title'][0] : $comments['title'][0],
      'author' => isset( $info['tags']['id3v2']['artist'][0] ) ? $info['tags']['id3v2']['artist'][0] : $comments['artist'][0],
      'album' => isset( $info['tags']['id3v2']['album'][0] ) ? $info['tags']['id3v2']['album'][0] : $comments['album'][0],
      'picture' => isset( $picture_path ) ? $picture_path : '',

    );

    $result = new FeedsParserResult();
    $result->items[] = $item;
    return $result;
  }

  /**
   * Add the extra mapping sources provided by this parser.
   */
  public function getMappingSources() {
    return parent::getMappingSources() + array(
      'title' => array(
        'name' => t('Media file title'),
      ),
      'album' => array(
        'name' => t('Album'),
      ),
      'author' => array(
        'name' => t('Author'),
      ),
      'Genre' => array(
        'name' => t('Genre'),
      ),

      'duration' => array(
        'name' => t('Duration (Formatted)'),
      ),
      'duration_raw' => array(
        'name' => t('Duration (Seconds)'),
      ),
      'file' => array(
        'name' => t('File Path'),
      ),
      'picture' => array(
        'name' => t('Picture'),
      ),

    );
  }


}
