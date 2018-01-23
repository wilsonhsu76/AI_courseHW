#include <stack>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

struct State {
	int state_index;
	int stacksNo;			// Total number of stacks
	int blocksNo;			// Total number of blocks
	vector < stack<char, vector<char>> > stacks;	// Blocks are in stacks
};

void init_State(State& s, int _state_index, int _stacksNo, int _blocksNo, vector < stack<char, vector<char>> > _stacks) {
	s.state_index = _state_index;
	s.stacksNo = _stacksNo;
	s.blocksNo = _blocksNo;
	//deep copy from _stacks to stacks
	s.stacks.resize(_stacksNo);
	vector<char> v;
	for (int i = 0; i < _stacksNo; i++) {
		//move stack's element to vector 
		int sz = _stacks[i].size();
		for (int k = 0; k < sz; k++) {
			char tmp = _stacks[i].top();
			v.push_back(tmp);
			_stacks[i].pop();
		}
		//move elements to stacks and ref_stacks 
		for (int j = sz-1; j >= 0; j--) {
			s.stacks[i].push(v[j]);
			_stacks[i].push(v[j]);
		}
		v.clear();
	}
}

//copy construct
void copy_State(State& s, State& s_ref) {
	s.state_index = s_ref.state_index;
	s.stacksNo = s_ref.stacksNo;
	s.blocksNo = s_ref.blocksNo;
	s.stacks.clear();
	s.stacks.resize(s.stacksNo);
	s.stacks.resize(s_ref.stacksNo);
	vector<char> v;

	for (int i = 0; i < s_ref.stacksNo; i++) {
		//move stack's element to vector 
		int sz = s_ref.stacks[i].size();
		for (int k = 0; k < sz; k++) {
			char tmp = s_ref.stacks[i].top();
			v.push_back(tmp);
			s_ref.stacks[i].pop();
		}
		//move elements to stacks and ref_stacks 
		for (int j = sz-1; j >=0 ; j--) {
			s.stacks[i].push(v[j]);
			s_ref.stacks[i].push(v[j]);
		}
		v.clear();
	}
}

struct Node {
	int h_cost;			    // h,  f = g + h
	int depth;				// g,  The path length from the initial state to the current state
	Node* parent;	        // Parent, shallow copy
	State node_state;
};

void init_Node(Node& n, int _h_cost, int _depth, Node* _parent, State _s) {
	n.h_cost = _h_cost;
	n.depth = _depth;
	n.parent = _parent;
	copy_State(n.node_state, _s);
}

void copy_Node(Node& n, Node& n_ref) {
		n.h_cost = n_ref.h_cost;
		n.depth = n_ref.depth;
		n.parent = n_ref.parent;
		copy_State(n.node_state, n_ref.node_state);
}

void print_states(vector < stack<char, vector<char>> > stacks) {
	fstream outf("proj1_record.txt", ios::out | ios::app);
	int stack_no = stacks.size();
	for (int i = 0; i < stack_no; i++) {
		int block_no = stacks[i].size();
		//vector<char> v = stacks[i]._Get_container();
		vector<char> v_res;
		vector<char> v;
		int sz = stacks[i].size();
		for (int k = 0; k < sz; k++) {
			char tmp = stacks[i].top();
			v_res.push_back(tmp);
			stacks[i].pop();
		}
		v.resize(sz);
		for (int k = sz - 1; k >= 0; k--) {
			v[sz - 1 - k] = v_res[k];
			stacks[i].push(v_res[k]);
		}

		//cout << (i + 1) << " | ";
		outf << (i + 1) << " | ";
		for (int j = 0; j < block_no; j++) {
			//cout << v[j] << " ";
			outf << v[j] << " ";
		}
		//cout << endl;
		outf << endl;
	}
	//cout << endl;
	outf << endl;
	outf.close();
}

//return the first index of the specific node in vector v
int find_in_vector(Node& n, vector<Node>& v) {
	State s = n.node_state;
	vector < stack<char, vector<char>> > ss = s.stacks;
	for (int i = 0; i < (int)v.size(); i++) {
		bool find_flag = true; 
		vector < stack<char, vector<char>> > v_ss = v[i].node_state.stacks;
		for (int j = 0; j < s.stacksNo; j++) {
			if (v_ss[j] != ss[j]) {
				find_flag = false;
				break;
			}
		}
		if (find_flag)
			return i;  //return ret as soon as possible;
	}
	return -1;
}

//compare 2 node state
bool compare_node_states(Node& n, Node& n_cmp){
	State s = n.node_state;
	State s_cmp = n_cmp.node_state;
	bool equal_flag = true;
	vector < stack<char, vector<char>> > ss = s.stacks;
	vector < stack<char, vector<char>> > ss_cmp = s_cmp.stacks;

	for (int j = 0; j < s.stacksNo; j++) {
		if (ss_cmp[j] != ss[j]) {
			equal_flag = false;
			break;
		}
	}
	return equal_flag; //return ret as soon as possible;
}