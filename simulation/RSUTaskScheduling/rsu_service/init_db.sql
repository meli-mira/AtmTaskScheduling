-- Create timetable table
CREATE TABLE timetable(
    timetable_id SERIAL PRIMARY KEY,
    year_mapping INTEGER[],
    days INTEGER,
    startDateOFCalendar DATE,
    ian INTEGER DEFAULT 31,
    feb INTEGER DEFAULT 28,
    mar INTEGER DEFAULT 31,
    apr INTEGER DEFAULT 30,
    may INTEGER DEFAULT 31,
    iun INTEGER DEFAULT 30,
    iul INTEGER DEFAULT 31,
    aug INTEGER DEFAULT 31,
    sep INTEGER DEFAULT 30,
    oct INTEGER DEFAULT 31,
    nov INTEGER DEFAULT 30,
    dec INTEGER DEFAULT 31
);

-- Create resources table
CREATE TABLE resources (
    resource_id SERIAL PRIMARY KEY,
    name VARCHAR(255),
    capacity VARCHAR(255),
    timetable_id INTEGER,
    CONSTRAINT fk_timetable FOREIGN KEY (timetable_id) REFERENCES timetable(timetable_id)
);

-- Create nodes table
CREATE TABLE nodes (
    node_id SERIAL PRIMARY KEY,
    name VARCHAR(255),
    capacity INTEGER,
    minPriority INTEGER,
    maxPriority INTEGER,
    timetable_id INTEGER,
    parent_node_id INTEGER,
    CONSTRAINT fk_timetable FOREIGN KEY (timetable_id) REFERENCES timetable(timetable_id),
    CONSTRAINT fk_parent_node FOREIGN KEY (parent_node_id) REFERENCES nodes(node_id)
);

-- Create tasks table
CREATE TABLE tasks (
    task_id SERIAL PRIMARY KEY,
    name VARCHAR(255),
    description VARCHAR(255),
    priority INTEGER,
    request DATE,
    deadline DATE,
    duration INTEGER,
    hasIssues BOOLEAN,
    hasBeenPlanned BOOLEAN,
    taskType VARCHAR(255),
    tasksubtype VARCHAR(255),
    node_id INTEGER,
    CONSTRAINT fk_node FOREIGN KEY (node_id) REFERENCES nodes(node_id)
);

-- Create notifications table
CREATE TABLE notifications (
    notification_id SERIAL PRIMARY KEY,
    type VARCHAR(255),
    description VARCHAR(255),
    task_id INTEGER,
    node_id INTEGER,
    CONSTRAINT fk_task_id FOREIGN KEY (task_id) REFERENCES tasks(task_id),
    CONSTRAINT fk_node_id FOREIGN KEY (node_id) REFERENCES nodes(node_id)
);

-- Create node_admins table
CREATE TABLE node_admins (
    admin_id SERIAL PRIMARY KEY,
    username VARCHAR(255),
    password VARCHAR(255),
    node_id INTEGER,
    CONSTRAINT fk_node_id FOREIGN KEY (node_id) REFERENCES nodes(node_id)
);

-- Create task_resource table
CREATE TABLE task_resource (
    id SERIAL PRIMARY KEY,
    task_id INTEGER,
    resource_id INTEGER,
    CONSTRAINT fk_task_id FOREIGN KEY (task_id) REFERENCES tasks(task_id),
    CONSTRAINT fk_resource_id FOREIGN KEY (resource_id) REFERENCES resources(resource_id)
);

-- Create scheduling table
CREATE TABLE scheduling (
    id SERIAL PRIMARY KEY,
    node_id INTEGER,
    task_id INTEGER,
    startDate DATE,
    endDate DATE,
    CONSTRAINT fk_task_id FOREIGN KEY (task_id) REFERENCES tasks(task_id),
    CONSTRAINT fk_node FOREIGN KEY (node_id) REFERENCES nodes(node_id)
);
