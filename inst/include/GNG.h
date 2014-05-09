/* 
 * File:   GNGInclude.h
 * Author: staszek
 *
 * Created on 12 sierpień 2012, 11:56
 */

#ifndef GNGINCLUDE_H
#define	GNGINCLUDE_H

#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/offset_ptr.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>




#include "GNGGlobals.h"


#include "UniformGrid.h"

#include "SHMemoryManager.h"
#include "GNGNode.h"

#include "GNGGraph.h"
#include "GNGDatabase.h"
#include "GNGDatabaseExtra.h"
#include "GNGAlgorithm.h"

#include "MemoryAllocator.h"

#include "DebugCollector.h"


#include "GNGDefines.h"

#include "GNGConfiguration.h"
#include "GNGServer.h"
#include "GNGClient.h"

#endif	/* GNGINCLUDE_H */

