<?hh
class CayleyQuery{
  public Vector<string> $Query = Vector{};
  public static Set<string> $ProvideResultFor = Set{'All','GetLimit'};
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
    return $Result['result'] === null ? [] : $Result['result'];
  }
  public function __call(string $Name, array<mixed> $Arguments){
    $LeArguments = Vector{};
    foreach($Arguments as $Argument){
      if($Argument instanceof Stringish){
        $LeArguments->add('"'.addslashes((string)$Argument).'"');
      }
    }
    if($LeArguments->count() !== 0){
      $this->Query->add($Name.'('.implode(', ',$LeArguments).')');
    } else {
      $this->Query->add($Name.'()');
    }
    if(self::$ProvideResultFor->contains($Name)){
      return $this->__execute((string) $this);
    } else {
      return $this;
    }
  }
}
