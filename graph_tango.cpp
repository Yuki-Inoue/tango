
// graph の test, core routine を実装。
// 後はこれをうまく wrap していけば良い。


enum Color { Black = 0, Gray, White };


// return: whether the test succeeded.
//         NOT EXPIRED == SUCCESS == true
//         but this is the tango_test's responsibility
bool test_node
(const Graph &g, const VertexIndexPropertyMap i_map,
 const boost::graph_traits<Graph>::vertex_descriptor v,
 std::vector<Color> &state)
{
  const int vi = get(i_map, v);

  if(state[vi] == Gray)
    throw(boost::not_a_dag());
  if(state[vi] == White)
    continue;
  state[vi] = Gray;

  const boost::graph_traits<Graph>::out_edge_iterator
    eit = out_edges(v, g).first;
  const int en = out_degree(v, g);

  std::vector<int> eorder(en);
  for(int i=0; i<en; ++i)
    eorder[i] = i;
  std::random_shuffle(eorder.begin(), eorder.end());

  for(int i=0; i<en; ++i){
    const edge e = *(eit + eorder[i]);
    const vertex dst = target(e, g);
    if(!test_node(g, i_map, dst, state))
      return false;
  }

  state[vi] = White;
  return tango_test(v);
}

void test_graph(const Graph &g, VertexIndexPropertyMap i_map){
  typedef boost::graph_traits<Graph>::vertices_size_type v_size;
  typedef boost::graph_traits<Graph>::vertex_descriptor vertex;
  typedef boost::graph_traits<Graph>::vertex_iterator v_iterator;
  const v_size vn = num_vertices(g);

  std::vector<int> vorder(vn);
  for(int i=0; i<vn; ++i)
    vorder[i] = i;
  std::random_shuffle(vorder.begin(), vorder.end());

  std::vector<Color> state(vn); // all black by default

  v_iterator vit = vertices(g).first;
  for(int i=0; i<vn; ++i){
    const vertex v = *(vit + vorder[i]);
    test_node(g, i_map, v, state);
  }
}
