<?hh //strict
type TypeCayleyEntry = shape(
  'subject' => string,
  'predicate' => string,
  'object' => string,
  'label' => string
);
class Cayley{
  public function __construct(public string $URL){

  }
  public function g():CayleyQuery{
    return new CayleyQuery($this);
  }
  public function write(string $subject, string $predicate, string $object, string $label = ''):void{
    $this->writeMulti([shape(
      'subject' => $subject,
      'predicate' => $predicate,
      'object' => $object,
      'label' => $label
    )]);
  }
  public function writeMulti(Traversable<TypeCayleyEntry> $Items):void{
    $Items = json_encode($Items);
    $CH = curl_init('http://'.$this->URL.'/api/v1/write');
    curl_setopt_array($CH,[
      CURLOPT_RETURNTRANSFER => true,
      CURLOPT_POST => true,
      CURLOPT_POSTFIELDS => $Items
    ]);
    curl_exec($CH);
    curl_close($CH);
  }
}
class CayleyQuery{
  public Vector<string> $Query = Vector{};
  public function __construct(public Cayley $Cayley){
    $this->Query->add('g');
  }
  public function __toString():string{
    return implode('.',$this->Query);
  }
  private function __arg(string $arg):string{
    if($arg !== ''){
      $arg = '"'.addslashes($arg).'"';
    }
    return $arg;
  }
  private function __execute(string $query):array<array<string,string>>{
    $CH = curl_init('http://'.$this->Cayley->URL.'/api/v1/query/gremlin');
    curl_setopt_array($CH,[
      CURLOPT_RETURNTRANSFER => true,
      CURLOPT_POST => true,
      CURLOPT_POSTFIELDS => $query
    ]);
    $Result = json_decode(curl_exec($CH),1);
    curl_close($CH);
    return $Result['result'];
  }
  public function V(string $arg = ''):this{
    $arg = $this->__arg($arg);
    $this->Query->add("V($arg)");
    return $this;
  }
  public function Out(string $arg = ''):this{
    $arg = $this->__arg($arg);
    $this->Query->add("Out($arg)");
    return $this;
  }
  public function In(string $arg = ''):this{
    $arg = $this->__arg($arg);
    $this->Query->add("In($arg)");
    return $this;
  }
  public function Both(string $arg = ''):this{
    $arg = $this->__arg($arg);
    $this->Query->add("Both($arg)");
    return $this;
  }
  public function All():array<array<string,string>>{
    $this->Query->add('All()');
    return $this->__execute((string) $this);
  }
}
