<?hh //strict
require(__DIR__.'/CayleyQuery.hh');
type TypeCayleyEntry = shape(
  'subject' => string,
  'predicate' => string,
  'object' => string,
  'label' => string
);
enum CayleyOp:string as string{
  WRITE = 'write';
  DELETE = 'delete';
}
class Cayley{
  public function __construct(public string $URL){

  }
  public function g():CayleyQuery{
    return new CayleyQuery($this);
  }
  public function write(string $subject, string $predicate, string $object, string $label = ''):void{
    $this->ProcessMulti(CayleyOp::WRITE, [shape(
      'subject' => $subject,
      'predicate' => $predicate,
      'object' => $object,
      'label' => $label
    )]);
  }
  public function delete(string $subject, string $predicate, string $object, string $label = ''):void{
    $this->ProcessMulti(CayleyOp::DELETE, [shape(
      'subject' => $subject,
      'predicate' => $predicate,
      'object' => $object,
      'label' => $label
    )]);
  }
  public function ProcessMulti(CayleyOp $Op, Traversable<TypeCayleyEntry> $Items):void{
    $Items = json_encode($Items);
    $CH = curl_init('http://'.$this->URL.'/api/v1/'.$Op);
    curl_setopt_array($CH,[
      CURLOPT_RETURNTRANSFER => true,
      CURLOPT_POST => true,
      CURLOPT_POSTFIELDS => $Items
    ]);
    curl_exec($CH);
    curl_close($CH);
  }
}
