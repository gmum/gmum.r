/*
 * File: GNGAlgorithm.cpp
 * Author: staszek
 *
 * Created on 11 sierpień 2012, 10:02
 */

#include "GNGAlgorithm.h"

using namespace std;

SHGNGNode ** GNGAlgorithm::LargestErrorNodesLazy() {
    SHGNGNode ** largest = new SHGNGNode*[2];

    SHGNGNode * gngnode;

    FOREACH(it, errorHeap.getLazyList()) {
        gngnode = &m_g[*it];

        errorHeap.insert(gngnode->nr, gngnode->error_new);
    }
    errorHeap.getLazyList().clear();


    ErrorNode max;
    do {
        max = errorHeap.extractMax();

        DBG(1, "GNGAlgorithm::LargestErrorLazy::found max " + to_string(max.i));

        SHGNGNode * gngnode = &m_g[max.i];


        if (gngnode->error_cycle != c) {
            FixErrorNew(gngnode);
            errorHeap.update(gngnode->nr, gngnode->error_new);
        } else {
            largest[0] = gngnode;
            int j = 0;
            double error = 0.0;

            FOREACH(edg, (largest[0]->edges)) {
                ++j;
                FixErrorNew(&m_g[edg->nr]);
                if (j == 1) {
                    largest[1] = &m_g[edg->nr];
                    error = largest[1]->error_new;
                    ;
                    continue;
                }


                double new_error = m_g[edg->nr].error_new;
                if (error < new_error) {
                    error = new_error;
                    largest[1] = &m_g[edg->nr];
                }
            }
            break;
        }
    } while (1);

    return largest;
}


GNGGraph<SHGNGNode>* GNGGraphAccessHack::pool=0;

GNGAlgorithm::GNGAlgorithm(GNGGraph<SHGNGNode> * g, GNGDatabase* db,
        GNGAlgorithmControl * control, double * boundingbox_origin,
        double * boundingbox_axis, double l, int max_nodes,
        int max_age, double alpha, double betha, double lambda,
        double eps_v, double eps_n
        ) :
m_g(*g), g_db(db), m_control(control), c(0), s(0),
m_max_nodes(max_nodes), m_max_age(max_age),
m_alpha(alpha), m_betha(betha), m_lambda(lambda),
m_eps_v(eps_v), m_eps_n(eps_n), ug(boundingbox_origin, boundingbox_axis, l),
m_density_threshold(0.1), m_grow_rate(1.5),
errorHeap() {
    m_toggle_uniformgrid = m_toggle_lazyheap = true;
    m_local_utility.resize(1);

    
    GNGGraphAccessHack::pool = &m_g;
    ug.setDistFunction(GNGGraphAccessHack::dist);
    m_betha_powers_size = m_lambda * 10;
    m_betha_powers = new double[m_betha_powers_size];
    REP(i, m_betha_powers_size) m_betha_powers[i] = std::pow(m_betha, (double) (i));

    m_betha_powers_to_n = new double[m_max_nodes * 2];
    REP(i, m_max_nodes * 2) m_betha_powers_to_n[i] = std::pow(m_betha, m_lambda * (double) (i));

}

void GNGAlgorithm::RandomInit() {

    GNGExample ex1 = g_db -> drawExample();
    GNGExample ex2 = g_db -> drawExample();

    int index = 0;
    while (ex2 == ex1 && index < 100) {
        ++index;
        ex2 = g_db -> drawExample();
    }

    m_g.newNode(ex1.getPositionPtr());
    m_g.newNode(ex2.getPositionPtr());

    if (m_toggle_uniformgrid) {
        ug.insert(m_g[0].position, 0);
        ug.insert(m_g[1].position, 1);
    }

    if (m_toggle_lazyheap) {
        SetErrorNew(&m_g[0], 0.0);
        SetErrorNew(&m_g[1], 0.0);
    }
    if (this->m_utility_option == BasicUtility) {
        set_utility(0, 0.001);
        set_utility(1, 0.001);
    }
}

void GNGAlgorithm::AddNewNode() {
    using namespace std;


    DBG(4, "GNGAlgorith::AddNewNode::start search");


    SHGNGNode ** error_nodes_new;
    if (m_toggle_lazyheap) error_nodes_new = LargestErrorNodesLazy();
    else error_nodes_new = LargestErrorNodes();

    DBG(4, "GNGAlgorith::AddNewNode::search completed");

    if (!error_nodes_new[0] || !error_nodes_new[1]) return;

    DBG(4, "GNGAlgorith::AddNewNode::search completed and successful");

    /*
double max_error=-1;
REP(i,m_g.getMaximumIndex()){
if(m_g[i]->occupied){
FixErrorNew(m_g[i]);
cout<<m_g[i]->error_new<<",";
max_error = std::max(max_error, m_g[i]->error_new);
}
}
cout<<endl;
if(max_error != error_nodes_new[0]->error_new) {
REPORT(max_error);REPORT(error_nodes_new[0]->nr);
REPORT(error_nodes_new[0]->error_new);
}
max_error=-1;
GNGNode ** error_nodes_new2=LargestErrorNodes();
     */
    /*
REP(i,m_g.getMaximumIndex()+1){
//cout<<m_g[i]->error<<",";
if(m_g[i]->occupied){
max_error = std::max(max_error, m_g[i]->error);
}
}
//cout<<endl;
if(max_error != error_nodes_new[0]->error) {
REPORT(max_error)
;REPORT(error_nodes_new[0]->nr);
REPORT(error_nodes_new[0]->error);
}
     */
    if (m_max_nodes <= m_g.getNumberNodes()) {
        delete[] error_nodes_new;
        return;
    }

    double position[GNG_DIM + 1]; //param
    for (int i = 0; i <= GNG_DIM; ++i) //param
        position[i] = (error_nodes_new[0]->position[i] + error_nodes_new[1]->position[i]) / 2;



    int new_node_index = m_g.newNode(&position[0]);

    if (m_toggle_uniformgrid) ug.insert(m_g[new_node_index].position, new_node_index);


    DBG(4, "GNGAlgorith::AddNewNode::added " + to_string(m_g[new_node_index]));

    m_g.removeEdge(error_nodes_new[0]->nr, error_nodes_new[1]->nr);


    DBG(3, "GNGAlgorith::AddNewNode::removed edge beetwen " + to_string(error_nodes_new[0]->nr) + " and" + to_string(error_nodes_new[1]->nr));


    DBG(2, "GNGAlgorithm::AddNewNode::largest error node after removing edge : " + to_string(*error_nodes_new[0]));


    m_g.addUDEdge(error_nodes_new[0]->nr, new_node_index);
    m_g.addUDEdge(new_node_index, error_nodes_new[1]->nr);


    DBG(3, "GNGAlgorith::AddNewNode::add edge beetwen " + to_string(error_nodes_new[0]->nr) + " and" + to_string(new_node_index));


    DBG(3, "GNGAlgorith::AddNewNode::add edge beetwen " + to_string(error_nodes_new[1]->nr) + " and" + to_string(new_node_index));

    if (!m_toggle_lazyheap) {
        DecreaseError(error_nodes_new[0]);
        DecreaseError(error_nodes_new[1]);
        SetError(&m_g[new_node_index], (error_nodes_new[0]->error + error_nodes_new[1]->error) / 2);
    } else {
        DecreaseErrorNew(error_nodes_new[0]);
        DecreaseErrorNew(error_nodes_new[1]);
        SetErrorNew(&m_g[new_node_index], (error_nodes_new[0]->error_new + error_nodes_new[1]->error_new) / 2);
    }

    if (this->m_utility_option == BasicUtility) {
        this->set_utility(new_node_index, 0.5 * (get_utility(error_nodes_new[0]->nr) + get_utility(error_nodes_new[1]->nr)));
    }
    //


    delete[] error_nodes_new;
}

void GNGAlgorithm::Adapt(GNGExample * ex) {

    Time t1(boost::posix_time::microsec_clock::local_time());



    DBG(4, "GNGAlgorith::Adapt::commence search");

    SHGNGNode * nearest[2];
    if (m_toggle_uniformgrid) {
        std::vector<int> nearest_index = ug.findNearest(&ex->position[0], 2); //TwoNearestNodes(ex->position);

        if (nearest_index[0] == nearest_index[1]) return; //something went wrong (-1==-1 też)

        nearest[0] = &m_g[nearest_index[1]];
        nearest[1] = &m_g[nearest_index[0]];
    } else {
        SHGNGNode ** tmp = TwoNearestNodes(ex->getPositionPtr());
        nearest[0] = tmp[0];
        nearest[1] = tmp[1];
        delete[] tmp;
    }
    Time t2(boost::posix_time::microsec_clock::local_time());

    TimeDuration dt = t2 - t1;

    times["adapt1"] += dt.total_microseconds();

    t1 = boost::posix_time::microsec_clock::local_time();



    DBG(4, "GNGAlgorith::Adapt::found nearest nodes to the drawn example " + to_string(*nearest[0]) + " " + to_string(*nearest[1]));


    double error = m_g.getDist(nearest[0]->position, ex->getPositionPtr());


    DBG(3, "GNGAlgorith::Adapt::increasing error");

    if (!m_toggle_lazyheap) IncreaseError(nearest[0], error);
    else IncreaseErrorNew(nearest[0], error);


    DBG(3, "GNGAlgorith::Adapt::accounted for the error");

    if (m_toggle_uniformgrid) ug.remove(nearest[0]->position);
    for (int i = 0; i <= GNG_DIM; ++i) {
        nearest[0]->position[i] += m_eps_v * (ex->getPositionPtr()[i] - nearest[0]->position[i]);
    }
    if (m_toggle_uniformgrid) ug.insert(nearest[0]->position, nearest[0]->nr);


    if (nearest[0]->edgesCount)
        FOREACH(edg, (nearest[0]->edges)) {
        if (m_toggle_uniformgrid) ug.remove(m_g[edg->nr].position);
        for (int i = 0; i <= GNG_DIM; ++i) { //param accounting
            m_g[edg->nr].position[i] += m_eps_n * (ex->position[i] - m_g[edg->nr].position[i]);
        }
        if (m_toggle_uniformgrid) ug.insert(m_g[edg->nr].position, edg->nr);
    }


    DBG(4, "GNGAlgorith::Adapt::position of the winner and neighbour mutated");

    if (!m_g.isEdge(nearest[0]->nr, nearest[1]->nr)) {
        m_g.addUDEdge(nearest[0]->nr, nearest[1]->nr);
        DBG(4, "GNGAlgorith::Adapt::added edge beetwen " + to_string(nearest[0]->nr) + " and " + to_string(nearest[1]->nr));
    }

    bool BYPASS = false;

    DBG(4, "GNGAlgorith::Adapt::commence scan of edges");

    FOREACH(edg, (nearest[0]->edges)) {
        // cout<<edg->nr<<",";
        if (BYPASS) {
            --edg;
            BYPASS = false;
        }

        edg->age++;
        edg->rev->age++;

        if (edg->nr == nearest[1]->nr) {
            edg->age = 0;
            edg->rev->age = 0;
        }

        if (edg->age > m_max_age) {

            DBG(3, "GNGAlgorith::Adapt::Removing aged edge " + to_string(nearest[0]->nr) + " - " + to_string(edg->nr));

            int nr = edg->nr;


            //Note that this is O(N), but average number of edges is very small, so it is OK
            ++edg;
            m_g.removeEdge(nearest[0]->nr, nr);



            if (m_g[nr].edgesCount == 0 && this->m_utility_option == None) {

                DBG(8, "GNGAlgorith:: remove node because no edges");

                FOREACH(edg, (m_g[nr].edges)) {
                    DBG(40, "WARNING: GNGAlgorithm:: edges count of neighbours of erased node, shouldn't happen! " + to_string(m_g[edg->nr].edgesCount));
                }

                if (m_toggle_uniformgrid) ug.remove(m_g[nr].position);


                DBG(8, "GNGAlgorithm::Adapt() Erasing node " + to_string<int>(nr));
                DBG(8, "GNGAlgorithm::Adapt() First coordinate " + to_string<double>(m_g[nr].position[0]));

                m_g.deleteNode(nr);
            }
            if (m_g[nearest[0]->nr].edgesCount == 0 && this->m_utility_option == None) {

                DBG(49, "WARNING: GNGAlgorithm::Adapt() remove node because no edges, shouldn't happen"); //Shouldn't happen

                if (m_toggle_uniformgrid) ug.remove(m_g[nearest[0]->nr].position);
                m_g.deleteNode(nearest[0]->nr);
                break;
            }

            DBG(3, "GNGAlgorith::Adapt::Removal completed");



            if (edg == nearest[0]->edges.end()) break; //error! should be after checking
            BYPASS = true; //foreach will add one anyway
        }
    }

    DBG(3, "GNGAlgorith::Adapt::Scan completed");



    //erase nodes
    if (this->m_utility_option == BasicUtility) {
        this->utility_criterion_check();
    }


    t2 = boost::posix_time::microsec_clock::local_time();
    dt = t2 - t1;

    times["adapt2"] += dt.total_microseconds();

    DBG(3, "GNGAlgorith::Adapt::leaving");

}

int GNGAlgorithm::CalculateAccumulatedError() {
    int maximumIndex = m_g.getMaximumIndex();
    m_accumulated_error = 0.0;

    if (this->m_toggle_lazyheap) {
        int maximumIndex = m_g.getMaximumIndex();
        m_accumulated_error = 0.0;

        REP(i, maximumIndex + 1) {

            if (m_g[i].occupied) {
                m_accumulated_error += m_g[i].error_new;
            }
        }
        return m_accumulated_error;
    } else {

        REP(i, maximumIndex + 1) {

            if (m_g[i].occupied) {
                m_accumulated_error += m_g[i].error;
            }
        }
        return m_accumulated_error;
    }
}

void GNGAlgorithm::TestAgeCorrectness() {
    int maximumIndex = m_g.getMaximumIndex();

    REP(i, maximumIndex + 1) {

        if (m_g[i].occupied && m_g[i].edgesCount) {

            FOREACH(edg, (m_g[i].edges)) {
                if (edg->age > m_max_age) {
                    cout << "XXXXXXXXXXXXXXXXX\n";
                    cout << (m_g[i]) << endl;
                }

            }
        }
    }
}

void GNGAlgorithm::ResizeUniformGrid() {

    DBG(6, "GNGAlgorithm::Resize Uniform Grid");

    ug.new_l(ug.getCellLength() / m_grow_rate);
    int maximum_index = m_g.getMaximumIndex();

    REP(i, maximum_index + 1) {
        if (m_g[i].occupied) ug.insert(m_g[i].position, m_g[i].nr);
    }
}

SHGNGNode ** GNGAlgorithm::LargestErrorNodes() {
    SHGNGNode ** largest = new SHGNGNode*[2];

    largest[0] = 0;
    largest[1] = 0;
    double error = -1.0;

    REP(i, m_g.getMaximumIndex() + 1) {
        if (m_g[i].occupied) {
            error = std::max(error, m_g[i].error);
        }
    }

    REP(i, m_g.getMaximumIndex() + 1) {
        if (m_g[i].occupied) {
            if (m_g[i].error == error) largest[0] = &m_g[i];
        }
    }

    if (largest[0]->edgesCount == 0) //{largest[0]->error=0; return largest;} //error?
    {
        m_g.deleteNode(largest[0]->nr);
        return largest;
    }

    int j = 0;

    FOREACH(edg, (largest[0]->edges)) {
        ++j;

        if (j == 1) {
            largest[1] = &m_g[edg->nr];
            error = largest[1]->error;
            continue;
        }


        double new_error = m_g[edg->nr].error;
        if (error < new_error) {
            error = new_error;
            largest[1] = &m_g[edg->nr];
        }
    }

    //
    return largest;
}

SHGNGNode ** GNGAlgorithm::TwoNearestNodes(const double * position) { //to the example

    //no unique_ptr in this C++, nearest is returned!
    SHGNGNode** nearest = new SHGNGNode*[2];

    nearest[0] = 0;
    nearest[1] = 0;

    int start_index = 0;

    while (!m_g[start_index].occupied) ++start_index;

    double dist = m_g.getDist(position, m_g[start_index].position);
    nearest[0] = &m_g[start_index];


    DBG(1, "GNGAlgorithm::starting search of nearest node from " + to_string(start_index));


    for (int i = start_index + 1; i <= m_g.getMaximumIndex(); ++i) { //another idea for storing list of actual nodes?

        if (m_g[i].occupied) {

            double new_dist = m_g.getDist(position, m_g[i].position);

            if (dist > new_dist) {
                dist = new_dist;

                nearest[0] = &m_g[i];
            }
        }
    }

    start_index = 0;

    while (!m_g[start_index].occupied || start_index == nearest[0]->nr) ++start_index;
    dist = m_g.getDist(position, m_g[start_index].position);
    nearest[1] = &m_g[start_index];

    for (int i = start_index + 1; i <= m_g.getMaximumIndex(); ++i) { //another idea for storing list of actual nodes?

        if (m_g[i].occupied && i != nearest[0]->nr) {

            double new_dist = m_g.getDist(position, m_g[i].position);

            if (dist > new_dist) {
                dist = new_dist;

                nearest[1] = &m_g[i];
            }
        }
    }


    DBG(1, "search successful and nearest[1]= " + to_string(nearest[1]));

    return nearest;
}

void GNGAlgorithm::runAlgorithm() { //1 thread needed to do it (the one that computes)
    c = 0;
    s = 0;
    boost::posix_time::microseconds work(1000), waitsleep(40);
    bool waited = false;
    Time t1, t2, t3, t4;
    TimeDuration dt;

    int size = g_db->getSize();
    DBG(3, "GNGAlgorithm::check size of the db " + to_string(size));

    boost::posix_time::millisec workTime(100);

    while (g_db->getSize() < 2) {
        ++c;
        boost::this_thread::sleep(workTime);

        if (c % 10 != 0) continue;
        int size = g_db->getSize();
        DBG(2, "GNGAlgorithm::check size of the db " + to_string(size));

        m_control->checkPause();
    }
    RandomInit();
    c = 0;


    DBG(3, "GNGAlgorithm::init successful, starting the loop");

    t3 = boost::posix_time::microsec_clock::local_time();
    int iteration = 0;
    while (1) {//(m_g.getNumberNodes() < m_max_nodes) {
        m_control->checkPause();
        for (s = 0; s < m_lambda; ++s) { //global counter!!
            ++iteration;

            DBG(0, "GNGAlgorithm::draw example");

            GNGExample ex = g_db->drawExample();

            Adapt(&ex);

            for (int i = 0; i <= m_g.getMaximumIndex(); ++i) { //another idea for storing list of actual nodes?
                if (m_g[i].occupied && m_g[i].edgesCount == 0 && m_utility_option == None) {
                    REPORT("Error at " + to_string<int>(i));
                }
            }


        }

        DBG(1, "GNGAlgorithm::add new node");

        t1 = boost::posix_time::microsec_clock::local_time();
        AddNewNode();
        if (ug.getDensity() > m_density_threshold && m_toggle_uniformgrid) ResizeUniformGrid();
        t2 = boost::posix_time::microsec_clock::local_time();
        dt = t2 - t1;
        times["resize"] += dt.total_microseconds();
        ++c; //epoch
        if (!m_toggle_lazyheap) DecreaseAllErrors();
        if (this->m_utility_option == BasicUtility) decrease_all_utility();

    }
    t4 = boost::posix_time::microsec_clock::local_time();
    dt = t4 - t3;
    REPORT(times["adapt1"]);
    REPORT(times["adapt2"]);
    REPORT(times["resize"]);
    REPORT(dt.total_microseconds() / (double) 1000000);
}

