#include "BlockWorld_Ds.h"
#include <queue>
#include <algorithm>
#include <random>
#include <time.h>
using namespace std;
#define MAX_ITER 20000

struct Node_compare {
	bool operator()(const Node& a, const Node&  b){
		if ((a.depth + a.h_cost) == (b.depth + b.h_cost))
		{
			return a.depth < b.depth;  //deeper depth first 
		}
		return (a.depth + a.h_cost) > (b.depth + b.h_cost);  //f_min first
	}
};

class BlockWorld {
public:
	int stacksNo;
	int blocksNo;
	Node root;
	vector < stack<char, vector<char>> > stacks;   //current state in BlockWorld
	vector < stack<char, vector<char>> > goal_state; //goal state
	priority_queue < Node, vector<Node>, Node_compare> node_queue; //for possible fronted group
	vector <Node> visited;  //with correct depth info/parent, the same addr with the org nodes
	int depth; //access where
	int heuristic_method;  // 0 is org(only from order), 1 is new method(consider more)

	void random_init() {
		srand((unsigned int)time(NULL));
		stacks.resize(stacksNo);
		for (int i = 0; i < blocksNo; i++) {
			int pos = rand() % (stacksNo);
			char input_ch = i + (int)('A');
			stacks[pos].push(input_ch);
		}
		//stacks.resize(stacksNo);
		/*stacks[0].push('B');
		stacks[1].push('C');
		stacks[1].push('E');
		stacks[2].push('A');
		stacks[2].push('D');*/
		/*stacks[0].push('D');
		stacks[1].push('E');
		stacks[1].push('F');
		stacks[1].push('I');
		stacks[1].push('J');
		stacks[2].push('B');
		stacks[2].push('G');
		stacks[3].push('C');
		stacks[3].push('H');
		stacks[4].push('A');*/
		/*stacks[1].push('B');
		stacks[1].push('E');
		stacks[1].push('K');
		stacks[1].push('A');
		stacks[1].push('F');
		stacks[2].push('J');
		stacks[2].push('I');
		stacks[2].push('L');
		stacks[2].push('C');
		stacks[3].push('D');
		stacks[3].push('H');
		stacks[3].push('G');*/
	}

	void prod_goal() {
		goal_state.resize(stacksNo);
		for (int i = 0; i < blocksNo; i++) {
			char input_ch = i + (int)('A');
			goal_state[0].push(input_ch);
		}
	}

	// add or update new node to node_queue/visited
	void successor(Node &current_n) {
		Node tmp_n, backup_tmp_n;
		copy_Node(tmp_n, current_n);
		tmp_n.depth = current_n.depth + 1;
		tmp_n.parent = &current_n;
		copy_Node(backup_tmp_n, tmp_n);
		int stack_no = current_n.node_state.stacksNo;
		for (int i = 0; i < stack_no; i++) {
			copy_Node(tmp_n, backup_tmp_n);
			vector < stack<char, vector<char>> > &tmp_s = tmp_n.node_state.stacks;
			if (tmp_s[i].size() != 0) {
				char tmp_c = tmp_s[i].top();
				tmp_s[i].pop();
				for (int j = 0; j < stack_no; j++) {
					if (i != j) {
						tmp_s[j].push(tmp_c);
						int find_index = find_in_vector(tmp_n, this->visited); //if visited yet?
						if(find_index < 0) {
							//new nodes, new states, clac h_cost in first time
							if (heuristic_method != 0) {
								int h_cost = this->heuristic_difference(tmp_n);
								tmp_n.h_cost = h_cost;
							}
							else {
								int h_cost = this->goal_difference(tmp_n);
								tmp_n.h_cost = h_cost;
							}
							this->node_queue.push(tmp_n);
							this->visited.push_back(tmp_n);
							//print_states(tmp_s);  //test
							tmp_s[j].pop();
						}
						else {
							//checek need update or not
							Node& exist_n = this->visited[find_index];
							if (tmp_n.depth < exist_n.depth) {
								exist_n.depth = tmp_n.depth;
								exist_n.parent = tmp_n.parent;
								update_node_info_in_priority_queue(tmp_n);
							}
							tmp_s[j].pop();
						}
					}
				}
			}
		}
	}

	//use node n info to update old info in priority queue
	void update_node_info_in_priority_queue(Node& n) {
		bool mod_flag = false;
		if (this->node_queue.size() == 0) {
			Node* new_node_ptr = new Node;
			copy_Node(*(new_node_ptr), n); 
			node_queue.push(*(new_node_ptr));  //because this cost is less than before, maybe new path
			return;
		}
		vector <Node> tmp_node_vec;  //pop non-target nodes to this tmp buffer

		while (mod_flag == true) {
			Node x = this->node_queue.top();  // x is from queue
			//State backup_sx;
			//copy_State(backup_sx, x.node_state);
			this->node_queue.pop();
			//copy_State(x.node_state, backup_sx);
			if (compare_node_states(n, x)) {
				x.depth = n.depth;
				x.parent = n.parent; //fcost 
				mod_flag = true;
				this->node_queue.push(x);
			}
			else {
				tmp_node_vec.push_back(x);
				if (node_queue.size() == 0) {
					Node* new_node_ptr = new Node; //add this node
					copy_Node(*(new_node_ptr), n);
					node_queue.push(*(new_node_ptr));  //because this cost is less than before, maybe new path
					mod_flag = true; //no find terminate while loop
				}
			}
		}

		//push non-target nodes to this tmp buffer
		while (tmp_node_vec.size() != 0) {
			Node x = tmp_node_vec[tmp_node_vec.size()-1];
			State backup_sx;
			copy_State(backup_sx, x.node_state);
			tmp_node_vec.pop_back();  //pop will lose states, restore from backup
			copy_State(x.node_state, backup_sx);
			node_queue.push(x);
		}
	}

	//calc difference between the state of input node and goal 
	int goal_difference(Node& n) {
		int ret = this->blocksNo;
		vector < stack<char, vector<char>> > &tmp_s = n.node_state.stacks;
		vector<char> v_res;
		vector<char> v;
		int sz = tmp_s[0].size();
		for (int k = 0; k < sz; k++) {
			char tmp = tmp_s[0].top();
			v_res.push_back(tmp);
			tmp_s[0].pop();
		}
		v.resize(sz);
		for (int k = sz-1; k >=0; k--) {
			v[sz-1-k] = v_res[k];
			tmp_s[0].push(v_res[k]);
		}

		for (int i = 0; i < (int) v.size(); i++) {
			char cmp_ch = (char)(i + (int)'A');
			if (v[i] == cmp_ch)
				ret--;
		}
		//cout << ret << endl; //test
		return ret;
	}

	//heuristic: h1: first stack need how much block remove, 
	//           h2: and move target element from other stack
	//           h3: out-of-order, h=h1+h2+h3
	//calc heuristic difference between the state of input node and goal 
	int heuristic_difference(Node& n) {
		//calc out of order first, and know which element should remove from first stack 
		int ret = 0;
		int out_order = this->blocksNo; //h3
		int stack1_remove_need = 0;
		int target_remove_need = 0;
		vector < stack<char, vector<char>> > &tmp_s = n.node_state.stacks;
		vector<char> v_res;
		vector<char> v;
		int sz = tmp_s[0].size();
		for (int k = 0; k < sz; k++) {
			char tmp = tmp_s[0].top();
			v_res.push_back(tmp);
			tmp_s[0].pop();
		}
		v.resize(sz);
		for (int k = sz - 1; k >= 0; k--) {
			v[sz - 1 - k] = v_res[k];
			tmp_s[0].push(v_res[k]);
		}

		bool cont_corr_flag = true;
		int from_A_correct_cnt = 0;  //stack1 has A,B...
		for (int i = 0; i < (int)v.size(); i++) {
			char cmp_ch = (char)(i + (int)'A');
			if (v[i] == cmp_ch) {
				out_order--;
				if (cont_corr_flag)
					from_A_correct_cnt++;
			}
			else
				cont_corr_flag = false;
		}
		v_res.clear();
		v.clear();
		stack1_remove_need = tmp_s[0].size() - from_A_correct_cnt; //obtain h1

		if (out_order != 0) {
			//find target char
			char target_ch = 'A' + from_A_correct_cnt;
			int find_index = -1;
			int stack_num = tmp_s.size();
			for (int i = 0; i < stack_num; i++) {
				int sz = tmp_s[i].size();
				for (int k = 0; k < sz; k++) {
					char tmp = tmp_s[i].top();
					v_res.push_back(tmp);
					tmp_s[i].pop();
				}
				for (int k = sz - 1; k >= 0; k--) {
					tmp_s[i].push(v_res[k]);
				}

				for (int k = 0; k < sz; k++) {
					if (v_res[k] == target_ch)
						find_index = k;
				}
				v_res.clear();
				if (find_index >= 0)
					break;
			}
			target_remove_need = (find_index >=0)? find_index : 0 ;
		}
		ret = target_remove_need + stack1_remove_need + out_order;
		//cout << "h1:" << stack1_remove_need << endl; //test
		//cout << "h2:" << target_remove_need << endl; //test
		//cout << "h3:" << out_order << endl; //test
		//cout << "h:" << ret << endl; //test
		return ret;
	}

	void back_trace(Node& n) {
		vector <Node> answer_path;
		answer_path.push_back(n);
		Node* tmp_n = &n;
		while (tmp_n->parent != NULL) {
			tmp_n = tmp_n->parent;
			answer_path.push_back(*(tmp_n));
		}
		int sz = answer_path.size();
		for (int i = sz - 1; i >= 0; i--) {
			fstream outf4("proj1_record.txt", ios::out | ios::app);
			outf4 << "depth:" << (sz - 1 - i)<< endl;
			outf4.close();
			print_states(answer_path[i].node_state.stacks);
		}
	}
};



int main() {
	fstream outf("proj1_record.txt", ios::out | ios::app);
	outf << "========[ New Test is coming ]========" << endl;	
	BlockWorld BlockWorld_Obj;
	cout << "enter stack number" << endl;
	cin >> BlockWorld_Obj.stacksNo;
	cout << "enter block number" << endl;
	cin >> BlockWorld_Obj.blocksNo;
	cout << "enter heuristic way (0:org, 1:designed)" << endl;
	cin >> BlockWorld_Obj.heuristic_method;
	outf << "stack number:" << BlockWorld_Obj.stacksNo << "  block number:" << BlockWorld_Obj.blocksNo << " heuristic:" << BlockWorld_Obj.heuristic_method << endl;
	outf.close();
	BlockWorld_Obj.random_init();  //init state to BlockWorld_Obj.stacks;
	BlockWorld_Obj.prod_goal();    //BlockWorld_Obj.goal_state
	
	print_states(BlockWorld_Obj.stacks);
	fstream outf2("proj1_record.txt", ios::out | ios::app);
	outf2 << "=====[ expected goal ]=====" << endl;
	outf2.close();
	print_states(BlockWorld_Obj.goal_state);
	
	State s1;
	init_State(s1, 0, BlockWorld_Obj.stacksNo, BlockWorld_Obj.blocksNo, BlockWorld_Obj.stacks);
	init_Node(BlockWorld_Obj.root, 0, 0, NULL, s1);
	if(BlockWorld_Obj.heuristic_method!= 0){
		int h_cost = BlockWorld_Obj.heuristic_difference(BlockWorld_Obj.root);
		BlockWorld_Obj.root.h_cost = h_cost;
	}
	else {
		int h_cost = BlockWorld_Obj.goal_difference(BlockWorld_Obj.root);
		BlockWorld_Obj.root.h_cost = h_cost;
	}
	BlockWorld_Obj.node_queue.push(BlockWorld_Obj.root);
	BlockWorld_Obj.visited.push_back(BlockWorld_Obj.root);

	bool arrive_goal = false;
	int iter_cnt = 1;
	int max_queue_size = 0;
	fstream outf3("proj1_record.txt", ios::out | ios::app);
	outf3 << endl;

	while (iter_cnt < MAX_ITER) {
		//cout << endl << endl;
		//cout << "iter#:" << iter_cnt << " queue_size:" << BlockWorld_Obj.node_queue.size() << endl;
		//monitor if loop still running
		if (iter_cnt % 500 == 0) {
			cout << "[processing] iter_cnt" << iter_cnt << endl;
		}

		outf3 << "iter#:" << iter_cnt << " queue_size:" << BlockWorld_Obj.node_queue.size() << endl;
		if ((int)BlockWorld_Obj.node_queue.size() > max_queue_size) {
			max_queue_size = BlockWorld_Obj.node_queue.size();
		}

		Node* arrive_node_ptr = new Node;
		//choose one node with lowest cost
		if (BlockWorld_Obj.node_queue.size() == 0)
			break;
		else {
			Node pop_node = BlockWorld_Obj.node_queue.top();
			copy_Node(*(arrive_node_ptr), pop_node);
			BlockWorld_Obj.node_queue.pop();
		}
		outf3 << "access node:  f=g+h=" << arrive_node_ptr->depth + arrive_node_ptr->h_cost << "  depth:" << arrive_node_ptr->depth << endl << endl;

		if (BlockWorld_Obj.goal_difference(*(arrive_node_ptr)) == 0) {
			cout << "[find solution]" << " path_depth: " << arrive_node_ptr->depth << " iter:" << iter_cnt << endl;
			cout << "max_queue_size:" << max_queue_size << "  finding possible nodes:" << BlockWorld_Obj.visited.size() << endl << endl;
			outf3 << "[find solution]" <<  " path_depth: " << arrive_node_ptr->depth << " iter:" << iter_cnt <<endl;
			outf3 << "max_queue_size:" << max_queue_size << "  finding possible nodes:" << BlockWorld_Obj.visited.size() << endl << endl;
			outf3.close();
	        arrive_goal = true;
			//cout << "[Finish]: " << endl;
			//print_states(arrive_node_ptr->node_state.stacks);
			//cout << "h_cost" << arrive_node_ptr->h_cost << endl;
			//cout << "total_cost" << arrive_node_ptr->h_cost + arrive_node_ptr->depth << endl;
			BlockWorld_Obj.back_trace(*(arrive_node_ptr));
			break;
		}
		/*if (iter_cnt == 6 || iter_cnt == 16) {
			int x = 5566;
		}*/
		//print_states(arrive_node_ptr->node_state.stacks);
		//cout << "depth:" << arrive_node_ptr->depth << endl;
		//cout << "h_cost" << arrive_node_ptr->h_cost << endl;
		//cout << "total_cost" << arrive_node_ptr->h_cost + arrive_node_ptr->depth << endl;
		BlockWorld_Obj.successor(*(arrive_node_ptr));
		iter_cnt++;
	}

	if (!arrive_goal) {
		if (iter_cnt < MAX_ITER) {
			cout << "[failure] without possible path, iter:" << iter_cnt << endl;
			outf3 << "[failure] without possible path, iter:" << iter_cnt << endl;
		}
		else {
			cout << "[failure] finding solution, iter cnt equals upper MAX_ITER:" << MAX_ITER << endl;
			outf3 << "[failure] finding solution, iter cnt equals upper MAX_ITER:" << MAX_ITER << endl;
		}
		outf3.close();
	}
	
	/*Node n2;
	init_Node(n2, 10, 0, NULL, s2);
	cout << "test_goal" << endl;
	BlockWorld_Obj.goal_difference(n2);*/

	/*Node n1,n2,n3;
	init_Node(n1, 10, 0, NULL, s1);
	init_Node(n2, 3, 0, &n1, s2);
	init_Node(n3, 150, 0, &n2, s2);

	BlockWorld_Obj.node_queue.push(n1);
	BlockWorld_Obj.node_queue.push(n2);
	BlockWorld_Obj.node_queue.push(n3);
	
	BlockWorld_Obj.visited.push_back(n1);
	BlockWorld_Obj.visited.push_back(n2);

	int x = find_in_vector(n1, BlockWorld_Obj.visited);
	x = find_in_vector(n2, BlockWorld_Obj.visited);
	x = find_in_vector(n3, BlockWorld_Obj.visited);*/

	/*stack <char, vector<char>> s;
	s.push('A');
	s.push('B');
	cout << s.top() << endl;
	vector<char> v = s._Get_container();
	s.pop();*/
	cout << "[complete] enter anything to terminate program ";
	cin.ignore();
	cin.get();
	return 0;
}