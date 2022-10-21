
#ifdef __cplusplus
extern "C"
{
#endif

<<<<<<< HEAD
#include "encryptsql/combine.h"
=======
#include "postgres.h" // 与 #include <iomanip> 冲突

#include "utils/palloc.h"
#include "utils/memutils.h"
>>>>>>> dbdev
#include "encryptsql/node2str.h"
#include "nodes/nodes.h"
#include "values.h"
#include "nodes/parsenodes.h"
#include "nodes/pg_list.h"

#ifdef __cplusplus
}
#endif

#include <string>

#include <sstream>

<<<<<<< HEAD
#include "encryptsql/utils.h"
=======
#include "utils.h"
>>>>>>> dbdev
using namespace std;

//////////////////////////////////////////////////
//// Declaration of private Function
/////////////////////////////////////////////////

static string _valuesLists2String(List *Values);
static string _caseExpr2String(CaseExpr *ce);
static string _constraint2String(List *cList);
static string _tableElts2String(List *tableElts);
static string _typeName2String(TypeName *typeName);
static string _constraintRawExpr2String(Node *expr);
static string _subLink2String(SubLink *sl);
static string _sortClause2String(List *sortList);
static string _groupClause2String(List *groupList);
static string _columnRef2String(ColumnRef *cref);
static string _functionCall2String(FuncCall *func);
static string _functionArgs2String(List *args);
static string _toStringDispatcher(Node *expr);
static string _aExpr2String(A_Expr *expr);
static string _aConst2String(A_Const *n);
static string _rangeVar2String(RangeVar *rv);
static string _joinExpr2String(JoinExpr *joinExpr);
static string _joinType2String(JoinType joinType);
static string _tableElts2String(List *tableElts);
static string _UpdateTargetList2String(List *targetList);
static string _fromClause2String(List *frmList);
static string _targetList2String(List *targetList);
static string _list2String(List *list);
static string _value2String(Value *v);
static string _explicitConstraint2String(Constraint *con);

static string _insertStmt2String(InsertStmt *parseTree);
static string _deleteStmt2String(DeleteStmt *parseTree);
static string _updateStmt2String(UpdateStmt *parseTree);
static string _selectStmt2String(SelectStmt *parseTree);
static string _createStmt2String(CreateStmt *parseTree);
static string _indexStmt2String(IndexStmt *parseTree);

static string _dropStmt2String(DropStmt *parseTree);

template <typename T>
static string _ltos(T l);
int _getOpOrder(const string &name);

//////////////////////////////////////////////////
//// End of Declaration of private Function
/////////////////////////////////////////////////

static string _caseExpr2String(CaseExpr *ce)
{
    string ans;
    ans += " CASE ";
    if (ce->arg)
        ans += _toStringDispatcher((Node *)ce->arg);
    ListCell *cell;
    foreach (cell, ce->args)
    {
        CaseWhen *cw = (CaseWhen *)lfirst(cell);
        ans += " WHEN ";
        ans += _toStringDispatcher((Node *)cw->expr);
        ans += " THEN ";
        ans += _toStringDispatcher((Node *)cw->result);
    }
    if (ce->defresult)
    {
        ans += " ELSE ";
        ans += _toStringDispatcher((Node *)ce->defresult);
    }

    ans += " END ";
    return ans;
}

static string _subLink2String(SubLink *sl)
{

    string ans;
    if (!sl->subselect)
        return ans;

    if (sl->subLinkType == EXPR_SUBLINK)
    {
        ans += "(" + _selectStmt2String((SelectStmt *)sl->subselect) + ")";
    }
    else if (sl->subLinkType == EXISTS_SUBLINK)
    {
        ans += "EXISTS(" + _selectStmt2String((SelectStmt *)sl->subselect) + ")";
    }
    else
    {
        errmsg("unhandled sublink type: %d in subLink2String()", (int)EXPR_SUBLINK);
    }

    return ans;
}

static string _sortClause2String(List *sortList)
{
    ListCell *sort;
    string ans;
    foreach (sort, sortList)
    {
        SortBy *n = (SortBy *)lfirst(sort);
        ans += _toStringDispatcher(n->node);
        string sortType = string(getSortTypeBy(n->sortby_dir));
        if (sortType.size())
            ans += " " + sortType;
        if (sort->next)
            ans += ", ";
    }
    return ans;
}

static string _groupClause2String(List *groupList)
{
    ListCell *group;
    string ans;
    foreach (group, groupList)
    {
        Node *n = (Node *)lfirst(group);
        if (IsA(n, ColumnRef))
        {
            ans += _columnRef2String((ColumnRef *)n);
        }
        else
        {
            errmsg("unrecognized node type: %d in _groupClause2String()", (int)nodeTag(n));
        }
        if (group->next)
            ans += ", ";
    }
    return ans;
}

static string _joinType2String(JoinType joinType)
{
    string ans;
    switch (joinType)
    {
    case JOIN_INNER:
        ans = "INNER JOIN";
        break;

    case JOIN_LEFT:
        ans = "LEFT JOIN";
        break;

    case JOIN_FULL:
        ans = "FULL JOIN";
        break;

    case JOIN_RIGHT:
        ans = "FULL JOIN";
        break;

    default:
        break;
    }
    return ans;
}

static string _joinExpr2String(JoinExpr *joinExpr)
{
    string ans;
<<<<<<< HEAD
=======
    //! BUG joinExpr->larg  joinExpr->rarg 都可能为null
>>>>>>> dbdev
    ans += _rangeVar2String((RangeVar *)joinExpr->larg) + " " + _joinType2String(joinExpr->jointype) + +" " +
           _rangeVar2String((RangeVar *)joinExpr->rarg);
    if (joinExpr->quals)
    {
        ans += " ON ";
        ans += _aExpr2String((A_Expr *)joinExpr->quals);
    }

    if (joinExpr->alias)
    {
        ans += " AS ";
        ans += joinExpr->alias->aliasname;
    }

    return ans;
}

static string _rangeVar2String(RangeVar *rv)
{
    string ans;
    if (rv->catalogname)
    {
        ans += rv->catalogname;
        ans += ".";
    }

    if (rv->schemaname)
    {
        ans += rv->schemaname;
        ans += ".";
    }

    if (rv->relname)
    {
        ans += rv->relname;
    }

    if (rv->alias)
    {
        ans += " AS ";
        ans += rv->alias->aliasname;
    }

    return ans;
}

static string _toStringDispatcher(Node *expr)
{
    string ans;
    if (!expr)
        return ans;
    switch (nodeTag(expr))
    {
    case T_ColumnRef:
        ans = _columnRef2String((ColumnRef *)expr);
        break;
    case T_A_Const:
        ans = _aConst2String((A_Const *)expr);
        break;
    case T_FuncCall:
        ans = _functionCall2String((FuncCall *)expr);
        break;
    case T_A_Expr:
        ans = _aExpr2String((A_Expr *)expr);
        break;

    // case T_TypeCast: // TODO: 未处理

    // 	break;
    case T_SubLink:
        ans = _subLink2String((SubLink *)expr);
        break;

    case T_CaseExpr:
        ans = _caseExpr2String((CaseExpr *)expr);
        break;
    case T_List:
        ans = _list2String((List *)expr);
        break;
    case T_RangeVar:
        ans = _rangeVar2String((RangeVar *)expr);
        break;
    case T_String:
    case T_Integer:
    case T_Float:
    case T_Null:
        ans = _value2String((Value *)expr);
        break;

    default:
        errmsg("unrecognized node type: %d in _toStringDispatcher()", (int)nodeTag(expr));
        break;
    }
    return ans;
}

static string _aExpr2String(A_Expr *expr)
{
    string name(getAExprOp(expr));
    string ans;
    string lexpr = _toStringDispatcher(expr->lexpr);
    string rexpr = _toStringDispatcher(expr->rexpr);
    string lbracket, rbracket;
    if (IsA(expr->lexpr, A_Expr) && _getOpOrder(name) > _getOpOrder(getAExprOp((A_Expr *)expr->lexpr)))
    {
        lbracket = "(";
        rbracket = ")";
    }

    ans += (lbracket + lexpr + rbracket + " " + name);

    lbracket = "";
    rbracket = "";

    if (IsA(expr->rexpr, A_Expr) && _getOpOrder(name) > _getOpOrder(getAExprOp((A_Expr *)expr->rexpr)))
    {
        lbracket = "(";
        rbracket = ")";
    }

    ans += (" " + lbracket + rexpr + rbracket);
    return ans;
}

static string _value2String(Value *v)
{

    if (IsA(v, Null))
    {
        return "NULL";
    }

    string ans;
    switch (nodeTag(v))
    {
    case T_Float:
    case T_String:
        ans += "\'" + string(strVal(v)) + "\'";
        break;
    case T_Integer:
        ans += _ltos(intVal(v));
        break;

    default:
        printf("ERROR: Unknown Value type");
        break;
    }
    return ans;
}

static string _aConst2String(A_Const *n)
{
    string ans;

    Value *v = &n->val;
    ans = _value2String(v);
    return ans;
}

template <typename T>
static string _ltos(T l)
{
    ostringstream os;
    os << l;
    string result;
    istringstream is(os.str());
    is >> result;
    return result;
}

inline int _getOpOrder(const string &name)
{

    if (name == "AND" || name == "OR")
        return 1;

    if (name == "=" || name == "<" || name == ">" || name == ">=" || name == "<=" || name == "<>" || name == "!~")
        return 1;

    if (name == "+" || name == "-")
        return 2;

    if (name == "*" || name == "/")
        return 3;

    return 0;
}

static string _functionArgs2String(List *args)
{
    string ans;
    ListCell *arg;
    foreach (arg, args)
    {

        Node *n = (Node *)lfirst(arg);
        ans += _toStringDispatcher(n);
        if (arg->next)
            ans += ", ";
    }
    return ans;
}

static string _functionCall2String(FuncCall *func)
{
    string ans;
    if (func->funcname)
    {
        ListCell *cell;
        foreach (cell, func->funcname)
        {
            ans += strVal(lfirst(cell));
            if (cell->next)
                ans += ".";
        }
    }

    if (func->args)
        ans += "(" + _functionArgs2String(func->args) + ")";

    else if (func->agg_star)
        ans += "(*)";

    else
        ans += "()";

    return ans;
}

static string _targetList2String(List *targetList)
{
    ListCell *o_target;
    string ans;
    foreach (o_target, targetList)
    {
        ResTarget *res = (ResTarget *)lfirst(o_target);
        ans += _toStringDispatcher(res->val);
        if (res->name)
        {
            ans += " AS ";
            ans += res->name;
        }

        if (o_target->next)
            ans += ", ";
    }
    return ans;
}

static string _columnRef2String(ColumnRef *cref)
{
    // TODO: unfinished
    stringstream ss;
    ListCell *p = list_head(cref->fields);
    int fieldsSize = list_length(cref->fields);
    bool is_star = IsA(lfirst(p), A_Star);
    if (is_star)
    {
        p = lnext(p);
        fieldsSize--;
    }

    for (int i = 0; i < fieldsSize; ++i)
    {
        char *colname = strVal(lfirst(p));
        ss << colname;
        if (i < fieldsSize - 1)
        {
            if (is_star)
                ss << ", "; // 如果是星号表达式，则里面存储的是星号的展开形式
            else
                ss << ".";
        }

        p = lnext(p);
    }
    return ss.str();
}

static string _fromClause2String(List *frmList)
{
    ListCell *from;
    string ans;
    foreach (from, frmList)
    {
        Node *n = (Node *)lfirst(from);
        if (IsA(n, RangeVar))
        {
            ans += _rangeVar2String((RangeVar *)n);
        }
        else if (IsA(n, RangeSubselect))
        {
            RangeSubselect *rss = (RangeSubselect *)n;
            ans += "(" + string(_selectStmt2String((SelectStmt *)rss->subquery)) + ")";
            if (rss->alias)
            {
                ans += " AS ";
                ans += rss->alias->aliasname;
            }
        }

        else if (IsA(n, RangeFunction))
        { // TEST: Not Known what is this;
        }
        else if (IsA(n, JoinExpr))
        {
            ans += _joinExpr2String((JoinExpr *)n);
        }

        else
        {
            errmsg("unrecognized node type: %d in encryptFromClause()", (int)nodeTag(n));
        }
        if (from->next)
            ans += ", ";
    }
    return ans;
}

static string _constraintRawExpr2String(Node *expr)
{
    string ans;
    switch (nodeTag(expr))
    {
    case T_A_Const:
        ans += _aConst2String((A_Const *)expr);
        break;

    case T_FuncCall:
    {
        FuncCall *func = (FuncCall *)(expr);
        char *funcname = strVal(llast(func->funcname));
        if (!strcmp(funcname, "now"))
            ans += "CURRENT_TIMESTAMP";
        break;
    }
    default:
        errmsg("unhandled node type %d in _constraintRawExpr2String()\n", nodeTag(expr));
        break;
    }
    return ans;
}

static string _constraint2String(List *cList)
{
    ListCell *constraintCell;
    string ans;
    foreach (constraintCell, cList)
    {
        Constraint *con = (Constraint *)lfirst(constraintCell);
        ans += " ";
        ans += constrType2Str(con->contype);
        if (con->raw_expr)
            ans += _constraintRawExpr2String((Node *)con->raw_expr);
    }
    return ans;
}

static string _typeName2String(TypeName *typeName)
{
    string ans;
    ans = strVal(llast(typeName->names));
    if (typeName->typmods)
    {
        A_Const *aconst = (A_Const *)linitial(typeName->typmods);
        int param = intVal(&aconst->val);
        ans += "(";
        ans += to_string(param);
        ans += ")";
    }
    return ans;
}

static string _tableElts2String(List *tableElts) // createStmt
{
    string ans;
    ListCell *columnDefCell;

    foreach (columnDefCell, tableElts)
    {
        Node *n = (Node *)lfirst(columnDefCell);

        if (IsA(n, ColumnDef))
        {
            ColumnDef *columnDef = (ColumnDef *)n;
            ans += columnDef->colname;
            ans += " " + _typeName2String(columnDef->typeName);
            ans += _constraint2String(columnDef->constraints);
        }
        else if (IsA(n, Constraint))
        {
            ans += _explicitConstraint2String((Constraint *)n);
        }

        if (columnDefCell->next)
            ans += ", ";
    }
    return ans;
}

static string _UpdateTargetList2String(List *targetList)
{
    ListCell *cell;
    string ans;
    foreach (cell, targetList)
    {
        ResTarget *res = (ResTarget *)lfirst(cell);
        ans += res->name;
        ans += "=";
        ans += _toStringDispatcher(res->val);
        if (cell->next)
            ans += ", ";
    }
    return ans;
}

static string _insertTargetList2String(List *targetList)
{
    string ans;
    ListCell *cell;
    foreach (cell, targetList)
    {
        ResTarget *res = (ResTarget *)lfirst(cell);
        ans += res->name;
        if (cell->next)
            ans += ", ";
    }
    return ans;
}

static string _valuesLists2String(List *Values)
{
    string ans = " VALUES ";
    ListCell *cell;
    foreach (cell, Values)
    {
        ans += "(";
        ans += _list2String((List *)lfirst(cell));
        ans += ")";
        if (cell->next)
            ans += ", ";
    }
    return ans;
}

static string _list2String(List *list)
{
    string ans;
    ListCell *cell;
    foreach (cell, list)
    {
        ans += _toStringDispatcher((Node *)lfirst(cell));
        if (cell->next)
            ans += ", ";
    }
    return ans;
}

static string _explicitConstraint2String(Constraint *con)
{

    ListCell *cell;
    List *kList = NIL;
    string ans;
    if (con->conname)
    {
        ans += " ";
        ans += con->conname;
        ans += " ";
    }
    ans += constrType2Str(con->contype);
    ans += " (";

    if (con->contype == CONSTR_PRIMARY || con->contype == CONSTR_UNIQUE)
    {
        kList = con->keys;
    }
    else if (con->contype == CONSTR_FOREIGN)
        kList = con->fk_attrs;

    foreach (cell, kList)
    {
        ans += strVal((Value *)lfirst(cell));
        if (cell->next)
            ans += ", ";
    }
    ans += ") ";

    if (con->pktable)
    {
        ans += "REFERENCES ";
        ans += _toStringDispatcher((Node *)con->pktable);
    }

    if (con->pk_attrs)
    {
        ans += "(";
        foreach (cell, con->pk_attrs)
        {
            ans += strVal((Value *)lfirst(cell));
            if (cell->next)
                ans += ", ";
        }
        ans += ")";
    }

    return ans;
}

static string _ATtype2String(AlterTableType t)
{

    switch (t)
    {
    case AT_AddColumn:
        return "Add Column"; /* add column */
    case AT_AddColumnRecurse:
        return "Add ColumnRecurse"; /* internal to commands/tablecmds.c */
    case AT_AddColumnToView:
        return "Add ColumnToView"; /* implicitly via CREATE OR REPLACE VIEW */
    case AT_ColumnDefault:
        return "ColumnDefault"; /* alter column default */
    case AT_DropNotNull:
        return "Drop Not Null"; /* alter column drop not null */
    case AT_SetNotNull:
        return "Set Not Null"; /* alter column set not null */
    case AT_SetStatistics:
        return "Set Statistics"; /* alter column set statistics */
    case AT_SetOptions:
        return "Set Options"; /* alter column set ( options ) */
    case AT_ResetOptions:
        return "Reset Options"; /* alter column reset ( options ) */
    case AT_SetStorage:
        return "Set Storage"; /* alter column set storage */
    case AT_DropColumn:
        return "Drop Column"; /* drop column */
    case AT_DropColumnRecurse:
        return "Drop Column Recurse"; /* internal to commands/tablecmds.c */
    case AT_AddIndex:
        return "Add Index"; /* add index */
    case AT_ReAddIndex:
        return "ReAdd Index"; /* internal to commands/tablecmds.c */
    case AT_AddConstraint:
        return "Add Constraint"; /* add constraint */
    case AT_AddConstraintRecurse:
        return "Add ConstraintRecurse"; /* internal to commands/tablecmds.c */
    case AT_ValidateConstraint:
        return "Validate Constraint"; /* validate constraint */
    case AT_ValidateConstraintRecurse:
        return "Validate ConstraintRecurse"; /* internal to commands/tablecmds.c */
    case AT_ProcessedConstraint:
        return "Processed Constraint";
    case AT_AddIndexConstraint:
        return "Add Index Constraint"; /* add constraint using existing index */
    case AT_DropConstraint:
        return "Drop Constraint"; /* drop constraint */
    case AT_DropConstraintRecurse:
        return "Drop ConstraintRecurse"; /* internal to commands/tablecmds.c */
    case AT_AlterColumnType:
        return "Alter Column Type"; /* alter column type */
    case AT_AlterColumnGenericOptions:
        return "Alter Column Options"; /* alter column OPTIONS (...) */
    case AT_ChangeOwner:
        return "Change Owner"; /* change owner */
    case AT_ClusterOn:
        return "Cluster On"; /* CLUSTER ON */
    case AT_DropCluster:
        return "Drop Cluster"; /* SET WITHOUT CLUSTER */
    case AT_AddOids:
        return "Add Oids"; /* SET WITH OIDS */
    case AT_AddOidsRecurse:
        return "Add OidsRecurse"; /* internal to commands/tablecmds.c */
    case AT_DropOids:
        return "Drop Oids"; /* SET WITHOUT OIDS */
    case AT_SetTableSpace:
        return "Set TableSpace"; /* SET TABLESPACE */
    case AT_SetRelOptions:
        return "Set RelOptions"; /* SET (...) -- AM specific parameters */
    case AT_ResetRelOptions:
        return "Reset RelOptions"; /* RESET (...) -- AM specific parameters */
    case AT_ReplaceRelOptions:
        return "Replace RelOptions"; /* replace reloption list in its entirety */
    case AT_EnableTrig:
        return "ENABLE TRIGGER"; /* ENABLE TRIGGER name */
    case AT_EnableAlwaysTrig:
        return "ENABLE ALWAYS TRIGGER "; /* ENABLE ALWAYS TRIGGER name */
    case AT_EnableReplicaTrig:
        return "ENABLE REPLICA TRIGGE"; /* ENABLE REPLICA TRIGGER name */
    case AT_DisableTrig:
        return "DISABLE TRIGGER"; /* DISABLE TRIGGER name */
    case AT_EnableTrigAll:
        return "ENABLE TRIGGER ALL"; /* ENABLE TRIGGER ALL */
    case AT_DisableTrigAll:
        return "DISABLE TRIGGER ALL "; /* DISABLE TRIGGER ALL */
    case AT_EnableTrigUser:
        return "ENABLE TRIGGER USER"; /* ENABLE TRIGGER USER */
    case AT_DisableTrigUser:
        return "DISABLE TRIGGER USER "; /* DISABLE TRIGGER USER */
    case AT_EnableRule:
        return "Enable Rule"; /* ENABLE RULE name */
    case AT_EnableAlwaysRule:
        return "Enable Always Rule"; /* ENABLE ALWAYS RULE name */
    case AT_EnableReplicaRule:
        return "Enable Replica Rule"; /* ENABLE REPLICA RULE name */
    case AT_DisableRule:
        return "Disable Rule"; /* DISABLE RULE name */
    case AT_AddInherit:
        return "Add Inherit"; /* INHERIT parent */
    case AT_DropInherit:
        return "Drop Inherit"; /* NO INHERIT parent */
    case AT_AddOf:
        return "Add Of"; /* OF <type_name> */
    case AT_DropOf:
        return "Drop Of"; /* NOT OF */
    case AT_GenericOptions:
        return "Generic Options"; /* OPTIONS (...) */
    case AT_ReAddConstraint:
        return "ReAdd Constraint"; /* internal to commands/tablecmds.c */
    }

    return "";
}

static string _alterTableCmd2String(List *cmds)
{
    string ans;
    ListCell *cell;

    foreach (cell, cmds)
    {
        AlterTableCmd *atcmd = (AlterTableCmd *)lfirst(cell);
        ans += " " + _ATtype2String(atcmd->subtype) + " ";

        if (IsA(atcmd, AlterTableCmd))
            ans += _explicitConstraint2String((Constraint *)atcmd->def);
    }
    return ans;
}

static string _insertStmt2String(InsertStmt *parseTree)
{
    string ans = "INSERT INTO ";
    ans += _rangeVar2String(parseTree->relation);
    if (parseTree->cols)
    {
        ans += " (";
        ans += _insertTargetList2String(parseTree->cols);
        ans += ") ";
    }
    if (parseTree->selectStmt)
    {
        SelectStmt *ss = (SelectStmt *)parseTree->selectStmt;
        ans += _selectStmt2String(ss);
    }
    return ans;
}
static string _deleteStmt2String(DeleteStmt *parseTree)
{
    string ans = "DELETE FROM ";
    ans += _rangeVar2String(parseTree->relation);
    if (parseTree->whereClause)
    {
        ans += " WHERE ";
        ans += _toStringDispatcher(parseTree->whereClause);
    }

    return ans;
}
static string _updateStmt2String(UpdateStmt *parseTree)
{
    string ans;
    ans += "UPDATE ";
    ans += _rangeVar2String(parseTree->relation);
    ans += " SET ";
    ans += _UpdateTargetList2String(parseTree->targetList);
    if (parseTree->fromClause)
        ans += " FROM " + _fromClause2String(parseTree->fromClause);
    if (parseTree->whereClause)
        ans += " WHERE " + _toStringDispatcher(parseTree->whereClause);
    return ans;
}
static string _selectStmt2String(SelectStmt *parseTree)
{
    if (parseTree->valuesLists)
        return _valuesLists2String(parseTree->valuesLists);

    string ans = "SELECT ";
    if (parseTree->targetList)
    {
        ans += _targetList2String(parseTree->targetList);
    }

    if (parseTree->fromClause)
    {
        ans += " FROM ";
        ans += _fromClause2String(parseTree->fromClause);
    }

    if (parseTree->whereClause)
    {
        ans += " WHERE ";
        Node *n = parseTree->whereClause;
        if (IsA(n, A_Expr))
        {
            ans += _aExpr2String((A_Expr *)n);
        }

        else if (IsA(n, FuncCall))
        {
            ans += _functionCall2String((FuncCall *)n);
        }
        else
            printf("unhandled where expr in where node2str\n");
    }

    if (parseTree->groupClause)
    {
        ans += " GROUP BY ";
        ans += _groupClause2String(parseTree->groupClause);
    }

    if (parseTree->havingClause)
    {
        // TEST: guess its a A_Expr
        ans += " HAVING ";
        if (IsA(parseTree->havingClause, A_Expr))
            ans += _aExpr2String((A_Expr *)parseTree->havingClause);
    }

    if (parseTree->sortClause)
    {
        ans += " ORDER BY ";
        ans += _sortClause2String(parseTree->sortClause);
    }

    if (parseTree->limitCount)
    {
        ans += " LIMIT ";
        ans += _aConst2String((A_Const *)parseTree->limitCount);
    }

    if (parseTree->limitOffset)
    {
        ans += " OFFSET ";
        ans += _aConst2String((A_Const *)parseTree->limitOffset);
    }

    if (parseTree->lockingClause)
    {
        ans += " FOR UPDATE ";
    }

    return ans;
}
static string _createStmt2String(CreateStmt *parseTree)
{
    string ans;
    ans += "CREATE TABLE ";
    if (parseTree->if_not_exists)
        ans += "IF NOT EXISTS ";
    if (parseTree->relation)
    {
        ans += _rangeVar2String(parseTree->relation) + "(";
    }
    if (parseTree->tableElts)
    {
        ans += _tableElts2String(parseTree->tableElts);
    }
    else
        ans += "table_exists int ";
    ans += ")";
    return ans;
}
static string _indexStmt2String(IndexStmt *parseTree)
{
    string ans = "CREATE ";
    if (parseTree->unique)
        ans += "UNIQUE ";
    ans += "INDEX ";
    ans += parseTree->idxname;
    ans += " ON ";
    ans += _rangeVar2String(parseTree->relation);

    if (parseTree->accessMethod)
    {
        ans += " USING ";
        ans += parseTree->accessMethod;
    }

    if (parseTree->indexParams)
    {
        ans += " (";
        ListCell *cell;
        foreach (cell, parseTree->indexParams)
        {
            IndexElem *elem = (IndexElem *)lfirst(cell);
            ans += elem->name;
            if (cell->next)
                ans += ", ";
            // newParseTree->indexParams = lappend(newParseTree->indexParams, elem);
        }
        ans += ")";
    }

    return ans;
}
static string _dropStmt2String(DropStmt *parseTree)
{
    string ans = "DROP ";
    if (parseTree->removeType == OBJECT_TABLE)
        ans += "TABLE ";
    else if (parseTree->removeType == OBJECT_SCHEMA)
        ans += "SCHEMA ";
    else
        ans += "UNIMPLEMENT ";

    // ans += _list2String(parseTree->objects);
    ListCell *cell;
    // ListCell* icell;
    foreach (cell, parseTree->objects)
    {
        Value *v = (Value *)linitial((List *)lfirst(cell));
        ans += strVal(v);
        if (cell->next)
            ans += ", ";
    }
    return ans;
}
static string _alterTableStmt2String(AlterTableStmt *parseTree)
{
    string ans = "ALTER TABLE ";

    if (parseTree->relation)
        ans += _toStringDispatcher((Node *)parseTree->relation);

    if (parseTree->cmds)
        ans += _alterTableCmd2String(parseTree->cmds);

    return ans;
}

<<<<<<< HEAD
const char *parseTree2str(Node *parseTree)
{
=======
static string explainStmt2String(ExplainStmt *parseTree)
{
    string ans = "EXPLAIN ";

    if (parseTree->options)
    {
        ListCell *cell = nullptr;
        ans += "(";
        foreach (cell, parseTree->options)
        {
            Node *option = (Node *)lfirst(cell);
            if (option->type == T_DefElem)
            {
                ans += ((DefElem *)option)->defname;
                if (cell->next)
                    ans += ", ";
            }
        }
        ans += ") ";
    }
    if (parseTree->query)
        ans += parseTree2str(parseTree->query);

    return ans;
}

const char *parseTree2str(Node *parseTree)
{
>>>>>>> dbdev
    string ans;
    switch (nodeTag(parseTree))
    {
    case T_InsertStmt:
        ans = _insertStmt2String((InsertStmt *)parseTree);
        break;

    case T_DeleteStmt:
        ans = _deleteStmt2String((DeleteStmt *)parseTree);
        break;

    case T_UpdateStmt:
        ans = _updateStmt2String((UpdateStmt *)parseTree);
        break;

    case T_SelectStmt:
        ans = _selectStmt2String((SelectStmt *)parseTree);
        break;

    case T_CreateStmt:
        ans = _createStmt2String((CreateStmt *)parseTree);
        break;

    case T_IndexStmt:
        ans = _indexStmt2String((IndexStmt *)parseTree);
        break;

    case T_DropStmt:
        ans = _dropStmt2String((DropStmt *)parseTree);
        break;

    case T_AlterTableStmt:
        ans = _alterTableStmt2String((AlterTableStmt *)parseTree);
        break;
<<<<<<< HEAD
=======

    case T_ExplainStmt:
        ans = explainStmt2String((ExplainStmt *)parseTree);
        break;
>>>>>>> dbdev
    default:
        break;
    }

    ans += ";";

    char *cstr_ans = (char *)palloc(ans.size() + 5);
    sprintf(cstr_ans, "%s", ans.c_str());

    return cstr_ans;
}
