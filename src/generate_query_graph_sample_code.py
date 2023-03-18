from numpy import random

'''
Generate query graphs based on a collection of data graphs.
    1. Select a data graph uniformly at random;
    2. Select a vertex uniformly at random from said graph;
    3. Starting from that vertex, perform a random walk;
    4. Maintain the graph created by the union of visited vertices and edges;
    5. When the desired query graph size is reached, terminate.
'''


def generate_query_graph_random_walk(data_graphs, query_graph_num, edge_num, maximum_steps=10000):
    data_graph_edge_num = []
    for data_graph in data_graphs:
        edge_sum = 0
        for edges in data_graph:
            edge_sum += len(edges)
        data_graph_edge_num.append(edge_sum / 2)

    random.seed()

    query_set = []
    query_set_map = []

    while len(query_set) < query_graph_num:
        data_graph_id = 0
        # Step 1.
        while True:
            data_graph_id = random.choice(len(data_graphs))
            if data_graph_edge_num[data_graph_id] >= edge_num:
                break

        data_graph = data_graphs[data_graph_id]

        # Step 2.
        start_vertex = random.choice(len(data_graph))

        # Step 3.
        edge_set = set()
        current_vertex = start_vertex

        random_steps = 0
        while len(edge_set) < edge_num:
            if len(data_graph[current_vertex]) == 0:
                break
            next_vertex = random.choice(data_graph[current_vertex])
            edge = (current_vertex, next_vertex)
            if current_vertex > next_vertex:
                edge = (next_vertex, current_vertex)

            if edge not in edge_set:
                edge_set.add(edge)

            current_vertex = next_vertex

            random_steps += 1
            if random_steps > maximum_steps:
                break

        if len(edge_set) == edge_num:
            query_set.append(list(edge_set))
            query_set_map.append(data_graph_id)
            print 'No {0} query graph has been generated.'.format(len(query_set))

    return query_set, query_set_map



'''
Generate query graphs based on a collection of data graphs.
    1. Select a data graph uniformly at random;
    2. Select a vertex uniformly at random from said graph;
    3. Starting from that vertex, perform BFS;
    4. Maintain the graph created by the union of visited vertices and edges between vertices;
    5. When the desired graph size (the number of edges) is reached, terminate.
    Note: degree threshold is 2.2 to generate cyclic queries in the experiments.
'''


def generate_query_graph_bfs_specified_edges(data_graphs, query_graph_num, edge_num, degree_threshold):
    data_graph_edge_num = []
    for data_graph in data_graphs:
        edge_sum = 0
        for edges in data_graph:
            edge_sum += len(edges)
        data_graph_edge_num.append(edge_sum / 2)

    random.seed()

    query_set = []
    query_set_map = []

    while len(query_set) < query_graph_num:
        data_graph_id = 0
        # Step 1.
        while True:
            data_graph_id = random.choice(len(data_graphs))
            if data_graph_edge_num[data_graph_id] >= edge_num:
                break

        data_graph = data_graphs[data_graph_id]

        # Step 2.
        start_vertex = random.choice(len(data_graph))

        # Step 3. BFS

        selected_vertices = []
        selected_edges = []
        visited = [False] * len(data_graph)
        vertex_queue = [start_vertex]
        visited[start_vertex] = True

        while vertex_queue:
            current_vertex = vertex_queue.pop(0)

            is_valid = False
            for u in selected_vertices:
                if u in data_graph[current_vertex]:
                    new_edge = (current_vertex, u)
                    if u < current_vertex:
                        new_edge = (u, current_vertex)

                    selected_edges.append(new_edge)
                    if len(selected_edges) == edge_num:
                        is_valid = True
                        break

            selected_vertices.append(current_vertex)

            if is_valid:
                break

            for neighbor in data_graph[current_vertex]:
                if not visited[neighbor]:
                    vertex_queue.append(neighbor)
                    visited[neighbor] = True

        if len(selected_edges) == edge_num:
            degree = len(selected_edges) * 2 / float(len(selected_vertices))

            if degree >= degree_threshold:
                print 'Vertex Number: {0}, Edge Number: {1}, Degree: {2}'.format(len(selected_vertices),
                                                                                 len(selected_edges), degree)
                print 'Data Graph Id: {0}, Start Vertex: {1}'.format(data_graph_id, start_vertex)
                query_set.append(selected_edges)
                query_set_map.append(data_graph_id)

                print 'No {0} query graph has been generated.'.format(len(query_set))

    return query_set, query_set_map