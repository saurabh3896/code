#include <iostream>
#include <algorithm>
#include <list>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
//
// Graphic = ellipse | GraphicList
//
// GraphicList = empty | Graphic GraphicList
//
////////////////////////////////////////////////////////////////////////////////
class graphic
{
public:
  virtual void print() = 0;
};

class composite_graphic: public graphic
{
public:
  virtual void print() {
    auto pr = [](graphic* g) { g->print(); };
    for_each(m_children.begin(), m_children.end(), pr);
  }

public:
  void add(graphic* shape) 
  {
    m_children.push_back(shape);
  }

  void remove(graphic* shape) 
  {
    m_children.remove(shape);
  }

  list<graphic*> m_children;
};

class ellipse: public graphic
{
public:
  ellipse(int id): m_id(id) {}

  virtual void print() {
    cout << "ellipse: " << m_id << "\n";
  }

private:
  int m_id;
};

int main(int argc, const char *argv[])
{
  ellipse           e1(10), e2(20), e3(30);
  composite_graphic cp1, cp2, all;

  cp1.add(&e1); cp1.add(&e2); cp1.add(&e3);
  cp2.add(&e1); cp2.add(&e2); cp2.add(&e3);

  all.add(&cp1);
  all.add(&cp2);
  all.print();

  return 0;
}
