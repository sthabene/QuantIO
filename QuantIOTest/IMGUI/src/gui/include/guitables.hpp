#pragma once
#include "gui.hpp"

//Store tables
void AddTables(ImVector<QuantIO::Window>& windows);

class TableBasics : public QuantIO::Window {
public: TableBasics() : Window("Table Basics", "TableColumn", false) {}
	  void DisplayContents() override;
};

class TableBackground : public QuantIO::Window {
public: TableBackground() : Window("Border & Background", "TableColumn", false) {}
	  void DisplayContents() override;
};

class TableResizable : public QuantIO::Window {
public: TableResizable() : Window("Resizable", "TableColumn", false) {}
	  void DisplayContents() override;
};

class TableReorderable : public QuantIO::Window {
public: TableReorderable() : Window("Reorderable", "TableColumn", false) {}
	  void DisplayContents() override;
};

class TablePadding : public QuantIO::Window {
public: TablePadding() : Window("Padding", "TableColumn", false) {}
	  void DisplayContents() override;
};

class TableSizing : public QuantIO::Window {
public: TableSizing() : Window("Sizing", "TableColumn", false) {}
      void DisplayContents() override;
};
class TableVerticalScrolling : public QuantIO::Window {
public: TableVerticalScrolling() : Window("Vertical Scrolling", "TableColumn", false) {}
      void DisplayContents() override;
};
class TableHorizontalScrolling : public QuantIO::Window {
public: TableHorizontalScrolling() : Window("Horizontal scrolling", "TableColumn", false) {}
      void DisplayContents() override;
};
class TableColumnsFlags : public QuantIO::Window {
public: TableColumnsFlags() : Window("Columns Flags", "TableColumn", false) {}
      void DisplayContents() override;
};
class TableColumnsWidths : public QuantIO::Window {
public: TableColumnsWidths() : Window("Columns Widths", "TableColumn", false) {}
      void DisplayContents() override;
};
class TableNestedTables : public QuantIO::Window {
public: TableNestedTables() : Window("Nested Tables", "TableColumn", false) {}
      void DisplayContents() override;
};
class TableRowHeight : public QuantIO::Window {
public: TableRowHeight() : Window("Row Height", "TableColumn", false) {}
      void DisplayContents() override;
};
class TableOuterSize : public QuantIO::Window {
public: TableOuterSize() : Window("Outer Size", "TableColumn", false) {}
      void DisplayContents() override;
};
class TableBackgroundColor : public QuantIO::Window {
public: TableBackgroundColor() : Window("Background Color", "TableColumn", false) {}
      void DisplayContents() override;
};
class TableTreeView : public QuantIO::Window {
public: TableTreeView() : Window("Tree View", "TableColumn", false) {}
      void DisplayContents() override;
};
class TableItemWidth : public QuantIO::Window {
public: TableItemWidth() : Window("Item Width", "TableColumn", false) {}
      void DisplayContents() override;
};
class TableCustomHeaders : public QuantIO::Window {
public: TableCustomHeaders() : Window("Custom Headers", "TableColumn", false) {}
      void DisplayContents() override;
};
class TableContextMenus : public QuantIO::Window {
public: TableContextMenus() : Window("Context Menus", "TableColumn", false) {}
      void DisplayContents() override;
};
class TableSyncedInstances : public QuantIO::Window {
public: TableSyncedInstances() : Window("Synced Instances", "TableColumn", false) {}
      void DisplayContents() override;
};
class TableSorting : public QuantIO::Window {
public: TableSorting() : Window("Sorting", "TableColumn", false) {}
      void DisplayContents() override;
};
class TableAdvanced : public QuantIO::Window {
public: TableAdvanced() : Window("Advanced", "TableColumn", false) {}
      void DisplayContents() override;
};

class TableSQL : public QuantIO::Window {
public: TableSQL() : Window("SQL", "TableColumn", false) {}
      void DisplayContents() override;
};


