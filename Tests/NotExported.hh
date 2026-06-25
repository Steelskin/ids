// Opt-out marker: a declaration immediately preceded by the macro named by
// -not-exported-macro is left un-annotated.
// RUN: %idt -export-macro IDT_TEST_ABI -not-exported-macro IDT_TEST_ABI_NOT_EXPORTED %s 2>&1 | %FileCheck %s
//
// Without -not-exported-macro the marker has no effect and every declaration is
// annotated as usual.
// RUN: %idt -export-macro IDT_TEST_ABI %s 2>&1 | %FileCheck -check-prefix=DISABLED %s

#define IDT_TEST_ABI_NOT_EXPORTED

// A declaration right after the marker's own #define must still be annotated:
// the marker is recognized via macro *expansion*, so the #define is not itself
// mistaken for a use.
// CHECK: NotExported.hh:[[@LINE+2]]:1: remark: unexported public interface 'fn_after_define'
// DISABLED: NotExported.hh:[[@LINE+1]]:1: remark: unexported public interface 'fn_after_define'
void fn_after_define();

// Marked: not annotated when the option is set, annotated when it is not.
// CHECK-NOT: unexported public interface 'marked_fn'
// DISABLED: NotExported.hh:[[@LINE+1]]:27: remark: unexported public interface 'marked_fn'
IDT_TEST_ABI_NOT_EXPORTED void marked_fn();

// An unmarked declaration immediately after a marked one must still be
// annotated (the marker only applies to the declaration it precedes).
// CHECK: NotExported.hh:[[@LINE+1]]:1: remark: unexported public interface 'unmarked_fn'
void unmarked_fn();

// The marker sits before the `extern` specifier, ahead of the insertion point.
// CHECK-NOT: unexported public interface 'marked_var'
// DISABLED: NotExported.hh:[[@LINE+1]]:34: remark: unexported public interface 'marked_var'
IDT_TEST_ABI_NOT_EXPORTED extern int marked_var;

// CHECK: NotExported.hh:[[@LINE+1]]:8: remark: unexported public interface 'unmarked_var'
extern int unmarked_var;

// On a record the marker sits between the class-key and the name. The whole
// class is pruned.
// CHECK-NOT: unexported public interface 'MarkedClass'
// CHECK-NOT: unexported public interface 'marked_method'
// DISABLED: NotExported.hh:[[@LINE+1]]:34: remark: unexported public interface 'MarkedClass'
struct IDT_TEST_ABI_NOT_EXPORTED MarkedClass { virtual void marked_method(); };

// CHECK: NotExported.hh:[[@LINE+1]]:8: remark: unexported public interface 'PlainClass'
struct PlainClass { virtual void v(); };
