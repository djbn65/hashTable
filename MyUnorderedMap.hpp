void get_identity(std::string my_id[]) {
  my_id[0] = "djbn65";
  my_id[1] = "12534397";
}

template <typename K, typename V>
MyUnorderedMap<K,V>::MyUnorderedMap() {
  data_size = reserved_size = 0;
  m_data = nullptr;
}

template <typename K, typename V>
MyUnorderedMap<K,V>::~MyUnorderedMap() {
  delete[] m_data;
}

template <typename K, typename V>
MyUnorderedMap<K,V>::MyUnorderedMap(const MyUnorderedMap<K, V> &source) {
  data_size = source.data_size;
  reserved_size = source.reserved_size;
  m_data = new MyPair<K, V>[reserved_size];
  for (int i = 0; i < reserved_size; i++)
    m_data[i] = source.m_data[i];
}

template <typename K, typename V>
MyUnorderedMap<K, V> & MyUnorderedMap<K,V>::operator=(const MyUnorderedMap<K, V> &source) {
  clear();
  data_size = source.data_size;
  reserved_size = source.reserved_size;
  m_data = new MyPair<K, V>[reserved_size];
  for (int i = 0; i < reserved_size; i++)
    m_data[i] = source.m_data[i];

  return *this;
}

template <typename K, typename V>
int MyUnorderedMap<K,V>::hash(const K &key) const {
  std::string tempkey = key;
  int asciiTotal = 0;
  for (int i = 0; i < tempkey.length(); i++)
    asciiTotal+=(int)tempkey[i];

  return asciiTotal%reserved_size;
}

template <typename K>
int linProbe(K& key, int i) {
  return i;
}

template <typename K, typename V>
V & MyUnorderedMap<K,V>::at(const K &key) {
  MyPair<K, V>* temp = find(key);
  if (temp == nullptr)
  {
    throw std::out_of_range("MyUnorderedMap::at");
  }
  return (temp->second);
}

template <typename K, typename V>
V & MyUnorderedMap<K,V>::operator[](const K &key) {
  MyPair<K, V>* temp = find(key);
  if (temp == nullptr)
  {
    insert(MyPair<K, V>(key));
    return ((find(key))->second);
  }
  else
  {
    return temp->second;
  }
}

template <typename K, typename V>
bool MyUnorderedMap<K,V>::empty() const {
  return data_size == 0;
}

template <typename K, typename V>
int MyUnorderedMap<K,V>::size() const {
  return data_size;
}

template <typename K, typename V>
void MyUnorderedMap<K,V>::clear() {
  if (m_data != nullptr)
    delete[] m_data;
  m_data = nullptr;
  data_size = reserved_size = 0;
}

template <typename K, typename V>
void MyUnorderedMap<K,V>::insert(const MyPair<K, V> &init_pair) {
  if (m_data == nullptr)
  {
    reserve(2);
  }
  else if ((((float)data_size)/((float)reserved_size)) >= 0.6)
    reserve(reserved_size*2);

  int pos = hash(init_pair.first);
  int home = pos;
  int counter = 1;

  while (m_data[pos].first != init_pair.first && m_data[pos].first != "EMPTY")
  {
    pos = (home+linProbe(init_pair.first,counter))%reserved_size;
    counter++;
  }

  if (m_data[pos].first == init_pair.first)
    return;
  else
  {
    m_data[pos] = init_pair;
    data_size++;
  }

  if ((((float)data_size)/((float)reserved_size)) >= 0.6)
    reserve(reserved_size*2);
}

template <typename K, typename V>
void MyUnorderedMap<K,V>::erase(const K &key) {
  if (m_data == nullptr)
  {
    return;
  }

  int pos = hash(key);
  int home = pos;
  int counter = 1;

  while (m_data[pos].first != key && m_data[pos].first != "EMPTY")
  {
    pos = (home+linProbe(key,counter))%reserved_size;
    counter++;
  }

  if (m_data[pos].first != "EMPTY")
  {
    m_data[pos].first = "TOMB";
    data_size--;
  }
  else
    return;

  if (data_size == 0)
  {
    delete[] m_data;
    m_data = nullptr;
    data_size = reserved_size = 0;
  }
  else if (data_size <= reserved_size*(0.1))
    reserve(reserved_size*(0.3));

  return;
}

template <typename K, typename V>
MyPair<K, V> * MyUnorderedMap<K,V>::find(const K &key) const {
  if (m_data == nullptr)
  {
    return nullptr;
  }

  int pos = hash(key);
  int home = pos;
  int counter = 1;

  while (m_data[pos].first != key && m_data[pos].first != "EMPTY")
  {
    pos = (home+linProbe(key,counter))%reserved_size;
    counter++;
  }

  if (m_data[pos].first == key)
    return &(m_data[pos]);
  else
    return nullptr;
}

template <typename K, typename V>
void MyUnorderedMap<K,V>::print() const {
  int dataSizeCount = 0;
  cout << "[";
  for (int i = 0; i < reserved_size; i++)
  {
    if (m_data[i].first != "EMPTY" && m_data[i].first != "TOMB")
    {
      cout << "(" << m_data[i].first << ": " << m_data[i].second << ")";
      dataSizeCount++;
      if (dataSizeCount < data_size)
        cout << ", ";
    }
  }
  cout << "]";
}

template <typename K, typename V>
int MyUnorderedMap<K,V>::count(const K &key) const {
  MyPair<K,V>* pair = find(key);
  if (pair == nullptr)
    return 0;
  else
    return 1;
}

template <typename K, typename V>
void MyUnorderedMap<K,V>::reserve(int new_cap) {
  if (new_cap > 0 && new_cap > data_size)
  {
    MyUnorderedMap<K,V> temp = *this;
    clear();
    m_data = new MyPair<K,V>[new_cap];
    reserved_size = new_cap;

    for (int i = 0; i < reserved_size; i++)
      m_data[i].first = "EMPTY";

    for (int i = 0; i < temp.reserved_size; i++)
    {
      if (temp.m_data[i].first != "EMPTY" && temp.m_data[i].first != "TOMB")
      {
        insert(temp.m_data[i]);
      }
    }
  }
  return;
}
