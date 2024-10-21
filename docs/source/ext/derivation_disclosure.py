from docutils import nodes
from docutils.parsers.rst import Directive, directives
from sphinx.transforms.post_transforms import SphinxPostTransform
from sphinx.util.nodes import NodeMatcher


DETAILS_TAG_CLASS_NAME = "derivation"
DETAILS_SUMMARY = "Derivation"


class details(nodes.Element, nodes.General):
    pass

class summary(nodes.TextElement, nodes.General):
    pass

def visit_details(self, node):
    self.body.append(f"<details class=\"{DETAILS_TAG_CLASS_NAME}\">")

def depart_details(self, node):
    self.body.append("</details>")

def visit_summary(self, node):
    self.body.append("<summary>")

def depart_summary(self, node):
    self.body.append("</summary>")

class DerivationDisclosure(Directive):
    required_arguments = 0
    final_argument_whitespace = True
    has_content = True
    option_spec = {
        "class": directives.class_option,
        "name": directives.unchanged,
    }
    def run(self):
        admonition = nodes.container(
                "",
                classes=self.options.get("classes", []),
                type="derivation_disclosure"
        )
        textnodes, messages = self.state.inline_text(
                DETAILS_SUMMARY,
                self.lineno
        )
        admonition += nodes.paragraph(DETAILS_SUMMARY, "", *textnodes)
        admonition += messages
        self.state.nested_parse(self.content, self.content_offset, admonition)
        self.add_name(admonition)
        return [admonition]

class DerivationDisclosureTransform(SphinxPostTransform):
    default_priority = 200
    def run(self):
        matcher = NodeMatcher(nodes.container, type="derivation_disclosure")
        for node in self.document.traverse(matcher):
            newnode = details(**node.attributes)
            newnode += summary("", "", *node[0])
            newnode.extend(node[1:])
            node.replace_self(newnode)

def setup(app):
    app.add_node(details, html=(visit_details, depart_details))
    app.add_node(summary, html=(visit_summary, depart_summary))
    app.add_directive("derivation_disclosure", DerivationDisclosure)
    app.add_post_transform(DerivationDisclosureTransform)
    return {
        "version": "0.1",
        "parallel_read_safe": True,
        "parallel_write_safe": True,
    }

