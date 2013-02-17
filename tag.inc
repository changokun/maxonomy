<?php

class tag {
	public $en; // english (United-States) text of the tag varchar 60
	public $en_GB; // english (British) text of the tag varchar 60
	public $es_LA; // spanish (Latin-American) text of the tag
	public $ja; // japanese text of the tag
	// public $zh; // chinese text of the tag - add or comment out as many of these as you want, but make a field in the table with the same name, and make sure you keep at least one.

	public static $supported_languages = array('en', 'es_LA', 'ja', 'en_GB'); // same as the props above.

	public $tag_id;

	private $loaded = false;
	private $saved = false;
	private $error = false;
	private $error_messages = array();

	private $language_count = 0;

	const SCHEMA = 'test';
	const TABLE_NAME = 'tag';
	const MAXONOMY_ID = 1;
	const MAXONOMY_PRIMARY_KEY_PROPERTY_NAME = 'tag_id';

	/**
	* pass me either a string (which will be presumed to be of the preferred language)
	* or an array of 'en' => 'fantastic', 'es_LA' => 'fantastico' - whose keys are all supported languages.
	*
	*/
	public function __construct() {
		// set obj props based on supported languages
		foreach(self::$supported_languages as $lang) $this->$lang = false;

		// variable args...
		$args = func_get_args();
		$args = array_shift($args);
		if(is_scalar($args) and ! empty($args)) {
			// a single string - assume it is of the preferred language.
			$this->{maxonomy::$preferred_language_locale} = $args;
		} elseif(is_array($args)) {
			// an array - check all teh keys
			// trim them all first.
			foreach($args as $key => $arg) $args[$key] = trim($arg);
			foreach(array_keys($args) as $lang) {
				if(in_array($lang, self::$supported_languages) and ! empty($args[$lang])) {
					$this->$lang = $args[$lang];
				}
			}
		} else {
			throw new Exception('bad tag args');
		}

		// did we get at least one good language?
		foreach(self::$supported_languages as $lang) if($this->$lang) $this->language_count ++;
		if($this->language_count == 0) throw new Exception('no viable strings for tag');

		// the act of instantiating a tag will either load the tag_id from the db - would we update partial records? or create a new record.
		$where_clause = array();
		$where_vars = array();
		foreach(self::$supported_languages as $lang) {
			if($this->$lang) {
				$where_clause[] = $lang . ' = %s';
				$where_vars[] = $this->$lang;
			}
		}
		$where_clause = array(implode(' OR ', $where_clause) => $where_vars);
		$query = new select_query('test', '*', self::SCHEMA . '.' . self::TABLE_NAME, $where_clause);

		if(false !== $result = db::q($query)) {
			//m::dump($query, 'looking for them');
			if($query->num_rows == 0) {
				// not found at all, add a new one.
				$data = array('string' => array());
				foreach(self::$supported_languages as $lang) {
					if($this->$lang) {
						$data['string'][$lang] = $this->$lang;
					} else {
						$data['null'][$lang] = NULL;
					}
				}

				$query = new insert_query('test', self::SCHEMA . '.' . self::TABLE_NAME, $data);
				m::dump($query, 'inserting');
				if(false !== $tag_id = db::q($query)) {
					$this->tag_id = (int) $tag_id;
				} else {
					throw new Exception('cannot insert into db.');
				}

			} elseif($query->num_rows == 1) {
				if($this->language_count == 1) {
					// jsut one hit. great. if we only have one language, that is our record.
				} else {
					// if we have more than one language, that means one or more of them pulled up the record, which means we are possibly (inadvertantly) adding a translation.
					// if one of the db fields is blank, but we have infor for it update the record with the new info
					// if one of the fields is different.... is it an update?
					$data = array('string' => array()); // prepping data for possible update query
					foreach(self::$supported_languages as $lang) {
						if($this->$lang) {
							m::dump('db: ' . $result[0][$lang] . ' | new tag: ' . $this->$lang);
							if(empty($result[0][$lang])) {
								$data['string'][$lang] = $this->$lang;
							} elseif($result[0][$lang] != $this->$lang) {
								// hmmm... data mismatch. just creating a tag with x, y could change a record from x, z to x, y... which could be weird, and start edit wars. so let's only do this if in update mode.
								if(maxonomy::$update_mode) $data['string'][$lang] = $this->$lang;
							}
						}
					}
					// if we found anything, let's hit it.
					if(count($data['string'])) {
						m::dump($data);
						$query = new update_query('test', self::SCHEMA . '.' . self::TABLE_NAME, $data, 'tag_id = ' . $result[0]['tag_id']);
						if(false !== db::q($query)) {
							// it worked. we do this silently. done.
						} else {
							throw new Exception('could not update db');
						}
						m::death($query);
						die("<hr>XXX<br>Died on line " . __LINE__ . " of " . __FILE__);
					}

				}
				// regardless, set our id.
				$this->tag_id = (int) $result[0]['tag_id'];

			} else {
				// did we query just one language?
				m::death($query);
				die("<hr>XXX<br>Died on line " . __LINE__ . " of " . __FILE__);
			}

		} else {
			throw new Exception('cannot select from db.');
		}

	}

	public static function get_tag_by_id($tag_id = NULL) {
		if(empty($tag_id) or ! is_numeric($tag_id) or ! $tag_id) {
			trigger_error('bad tag id', E_USER_WARNING);
			return false;
		}

		$tag = new tag('temp'); // temp because you can't instantiate by id... need something to fill it
		$tag->tag_id = (int) $tag_id;
		$tag->load();

		if($tag->error) return false; // if it couldn't load, i don't want to return 'temp'

		return $tag;
	}

	private function load() { // arbitrary privacy
		$this->loaded = false;

		if( ! isset($this->tag_id) or ! is_numeric($this->tag_id) or ! $this->tag_id) {
			$this->error = true;
			$this->error_messages[] = 'bad tag id for load.';
			return false;
		}

		// run queries
		$query = new select_query('test', NULL, self::SCHEMA . '.' . self::TABLE_NAME, 'tag_id = ' . $this->tag_id);
		if(maxonomy::$admin_mode) {
			die("<hr>XXX<br>Died on line " . __LINE__ . " of " . __FILE__);
		} else {
			$query->clauses['select'] = maxonomy::$preferred_language_locale;
		}
		if(false !== $result = db::q($query)) {
			$this->en = $result[0][maxonomy::$preferred_language_locale];
			$this->loaded = true;
		} else {
			return false;
		}
	}
}


/**
* using this for when we have all the data, say from a query, and want a tag object without loading it.
*/
class tag_internal_use_only extends tag {

	/**
	* i'm expecting a tag_id and at least one tag_language & text pair.
	* i'm trusting that you loaded this stuff, and that it is accurate.
	*/
	public function __construct() {
		// set obj props based on supported languages
		foreach(self::$supported_languages as $lang) $this->$lang = false;

		// loaded, right?
		$this->loaded = true;

		// variable args...
		$args = func_get_args();
		$args = array_shift($args);


		if(is_array($args)) {
			// get id first
			if(isset($args['tag_id']) and is_numeric($args['tag_id']) and $args['tag_id']) {
				$this->tag_id = (int) $args['tag_id'];
				unset($args['tag_id']);
			} else {
				throw new Exception('bad id on tag init');
			}

			// everything else is a lang => tagText. trim them all first.
			foreach($args as $key => $arg) $args[$key] = trim($arg);
			$got_at_least_one_good_string = false;
			foreach(array_keys($args) as $lang) {
				if(in_array($lang, self::$supported_languages) and ! empty($args[$lang])) {
					$this->$lang = $args[$lang];
					$got_at_least_one_good_string = true;
				}
			}
		} else {
			throw new Exception('bad tag args - internal yuse only');
		}

		// as long as we got one good string, i'm happy.
		if( ! $got_at_least_one_good_string) throw new Exception('bad internal args for tags.');
	}
}